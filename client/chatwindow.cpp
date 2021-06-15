#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(const std::string ip,const std::string port,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow),
    _work{new boost::asio::io_service::work{_io_service}},
    _io_service_runner{[this]{ _io_service.run(); }},
    _client {_io_service, ip, port}
{
    ui->setupUi(this);
    this->show();
    _auth = new Authorize;
    _auth->show();

    this->setWindowTitle("Skull&Bones");
    this->centralWidget()->setStyleSheet("QWidget#centralwidget {background-image:url(:/images/fon_2.png);}");

    _add_user = new AddUser;

    _l = new QVBoxLayout;
    _l->addWidget(ui->listWidget);
    setLayout(_l);
    makeItem(ui->listWidget, "New");

    ui->stackedWidget_2->setCurrentIndex(0);

    connect(&_client, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(_auth, SIGNAL(do_auth(QString, QString)), this, SLOT(do_auth(QString, QString)));
    connect(&_client, SIGNAL(auth_complete(bool)), this, SLOT(is_auth_success(bool)));
    connect(_add_user, SIGNAL(add_new(std::string, std::string)), &_client, SLOT(add_user(std::string, std::string)));
    connect(this, SIGNAL(choose_room(std::string, std::string, std::string)), &_client, SLOT(choose_room(std::string, std::string, std::string)));
    connect(&_client, SIGNAL(groups(QList<QString>)), this, SLOT(display_groups(QList<QString>)));
    connect(&_client, SIGNAL(new_msg(std::string, std::string, std::string, int)), this, SLOT(display_msg(std::string, std::string, std::string, int)));
    connect(&_client, SIGNAL(group(QString,QString)), this, SLOT(group(QString,QString)));
    connect(&_client, SIGNAL(participants(std::string,std::list<std::string>)), this, SLOT(participants(std::string,std::list<std::string>)));
    connect(&_client, SIGNAL(friends(int,std::string)), this, SLOT(friends(int,std::string)));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::do_auth(QString login, QString pass)
{
    QByteArray hex_pass_ba = pass.toUtf8();
    QString hex_pass_qs(QCryptographicHash::hash(hex_pass_ba, QCryptographicHash::Sha3_512).toHex());
    _client.do_login(login.toStdString(), hex_pass_qs.toStdString());
}

void ChatWindow::is_auth_success(bool flag)
{
   if(flag)
       _done = true; //это вообще надо?
   else
   {
       _auth = new Authorize;
        connect (_auth, SIGNAL(do_auth(QString, QString)), this, SLOT(do_auth(QString, QString)));
       _auth->show();
   }
}

void ChatWindow::on_actionAdd_triggered()
{
    _add_user->show();
}

void ChatWindow::makeItem( QListWidget* lstWgt, QString i)
{
    list_item * name = new list_item(i,0);
    QListWidgetItem* item = new QListWidgetItem( lstWgt );
    item->setSizeHint( name->size() );
    lstWgt->setItemWidget( item, name);
    _items.insert(std::pair<QString, QListWidgetItem *>(i, item));

    if (i!="New")
    {
        Letters * ltrs = new Letters(i, this);
        ui->stackedWidget->addWidget(ltrs);
        connect(ltrs, SIGNAL(send_msg(QString, QString)), this, SLOT(send_msg(QString, QString)));
        connect(ltrs, SIGNAL(group(QString,QString)), this, SLOT(group(QString, QString)));
        connect(ltrs, SIGNAL(participants(QString)), &_client, SLOT(get_participants(QString)));
        connect(ltrs, SIGNAL(add_to_group(QString, QString)), &_client, SLOT(add_to_group(QString, QString)));
        _chats.insert(std::pair<QString, Letters*>(i, ltrs));
    }
}

void ChatWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    list_item * widget = qobject_cast<list_item*>(ui->listWidget->itemWidget(item));
    if (widget->get_name() == "New")
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        auto chat = _chats.find(widget->get_name());
        ui->stackedWidget->setCurrentWidget(chat->second);
    }
}

void ChatWindow::on_lineEdit_2_returnPressed()
{
    QString _name = ui->lineEdit_3->text();
    QString _participants = ui->lineEdit_2->text();
    if ((_name.length() != 0) && (_participants.length() != 0))
    {
        emit choose_room(std::string("new"), _name.toStdString(), _participants.toStdString());
    }
}

void ChatWindow::error(QString str)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    if (str == "Not connected")
    {
        msgBox.setText("Connection is unseccessfull!");
        msgBox.exec();
        this->close();
        _auth->close();
        Settings * set = new Settings;
        set->show();
    }
    else
    {
        msgBox.setText(str);
        msgBox.exec();
    }
}

void ChatWindow::display_groups(QList<QString> groups)
{
   for (const auto & x: groups)
       makeItem(ui->listWidget, x);
}

void ChatWindow::send_msg(QString name, QString text)
{
    chat_message msg;
    msg.type = chat_message::NEW_MSG;
    msg.list_string.push_back(name.toStdString());
    msg.login = _client.get_login();
    msg.message = text.toStdString();
    _client.do_write(msg);
}

void ChatWindow::display_msg(std::string name, std::string login, std::string msg, int flag)
{
    auto chat = _chats.find(QString::fromStdString(name));
    chat->second->publish(login, msg, flag);
}

void ChatWindow::group(QString group_name, QString factor)
{
    if (factor == "delete")
    {
        auto p = _chats.find(group_name);
        if (p != _chats.end())
        {
            ui->stackedWidget->setCurrentIndex(0);
            ui->stackedWidget->removeWidget(p->second);
            auto pi = _items.find(group_name);
            ui->listWidget->setCurrentItem(_items.find("New")->second);
            delete p->second;
            int row = ui->listWidget->row(pi->second);
            ui->listWidget->takeItem(row);
            delete pi->second;
            _items.erase(pi);
            _chats.erase(group_name);
            chat_message e;
            e.type = chat_message::TABLE;
            e.message = "Delete";
            e.list_string.push_back(group_name.toStdString());
            _client.do_write(e);
        }
    }
    if (factor == "exit")
    {
        auto p = _chats.find(group_name);
        if (p != _chats.end())
        {
            ui->stackedWidget->setCurrentIndex(0);
            ui->stackedWidget->removeWidget(p->second);
            auto pi = _items.find(group_name);
            ui->listWidget->setCurrentItem(_items.find("New")->second);
            delete p->second;
            int row = ui->listWidget->row(pi->second);
            ui->listWidget->takeItem(row);
            delete pi->second;
            _items.erase(pi);
            _chats.erase(group_name);
            chat_message e;
            e.login = _client.get_login();
            e.type = chat_message::TABLE;
            e.message = "Exit";
            e.list_string.push_back(group_name.toStdString());
            _client.do_write(e);
        }
    }
}

void ChatWindow::participants(std::string group_name ,std::list<std::string> names)
{
    auto chat = _chats.find(QString::fromStdString(group_name));
    chat->second->display_particiants(names);
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    chat_message e;
    e.type = chat_message::PERSON_LEFT;
    e.login = _client.get_login();
    _client.do_write(e);
    _add_user->close();
}

void ChatWindow::on_actionfriends_triggered()
{
    ui->stackedWidget_2->setCurrentIndex(1);
    chat_message e;
    e.type = chat_message::FRIEND;
    e.login = _client.get_login();
    e.message = "Get";
    _client.do_write(e);
    _menu = new QMenu(this);
    QAction* deleteAction = _menu->addAction("Delete");
    connect( deleteAction, SIGNAL( triggered() ), this, SLOT(delete_friend()));
    QAction* startAction = _menu->addAction("New Message");
    connect( startAction, SIGNAL( triggered() ), this, SLOT(start_message()));
}

void ChatWindow::on_lineEdit_returnPressed()
{
    std::string name = ui->lineEdit->text().toStdString();
    if (!(_friends.find(QString::fromStdString(name)) != _friends.end()))
        if (name != _client.get_login())
            _client.add_friend(ui->lineEdit->text().toStdString());
}

void ChatWindow::friends(int flag ,std::string from)
{
    if (flag == 1)
    {
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Requesr", "User "+QString::fromStdString(from)+" wants to add you in friends",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                chat_message e;
                e.type = chat_message::FRIEND;
                e.login = _client.get_login();
                e.list_string.push_back("Good");
                e.message = from;
                _client.do_write(e);
                _friends.insert(QString::fromStdString(from));
                QListWidgetItem* listItem = new QListWidgetItem(QString::fromStdString(from));
                listItem->setIcon( QPixmap(":/images/online.png") );
                listItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
                ui->listWidget_2->addItem( listItem );
            }
        }
    }
    if (flag == 11)
    {
        if (!(_friends.find(QString::fromStdString(from)) != _friends.end()))
        {
            _friends.insert(QString::fromStdString(from));
            QListWidgetItem* listItem = new QListWidgetItem(QString::fromStdString(from));
            listItem->setIcon( QPixmap(":/images/online.png") );
            listItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
            ui->listWidget_2->addItem( listItem );
        }
    }
    if (flag == 0)
    {
        if (!(_friends.find((QString::fromStdString(from).section(',',1))) != _friends.end()))
        {

            _friends.insert(QString::fromStdString(from).section(',',1));
            QListWidgetItem* listItem = new QListWidgetItem(QString::fromStdString(from).section(',',1));
            if (from.at(0) == '1')
                listItem->setIcon( QPixmap(":/images/online.png") );
            if (from.at(0) == '0')
                listItem->setIcon( QPixmap(":/images/offline.png") );
            listItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
            ui->listWidget_2->addItem( listItem );
        }
        else
        {
            auto p = ui->listWidget_2->findItems(QString::fromStdString(from).section(',',1), Qt::MatchExactly);
            if (from.at(0) == '1')
                p.at(0)->setIcon( QPixmap(":/images/online.png") );
            if (from.at(0) == '0')
                p.at(0)->setIcon( QPixmap(":/images/offline.png") );

        }
    }
    if (flag == 111)
    {
         if (_friends.find((QString::fromStdString(from).section(',',1))) != _friends.end())
        {
             QMessageBox msgBox;
                     msgBox.setWindowTitle("NoError");
                     msgBox.setText("Work!");
                     msgBox.exec();
            auto p = ui->listWidget_2->findItems(QString::fromStdString(from).section(',',1), Qt::MatchExactly);
            if (from.at(0) == '1')
                p.at(0)->setIcon( QPixmap(":/images/online.png") );
            if (from.at(0) == '0')
                p.at(0)->setIcon( QPixmap(":/images/offline.png") );

        }
    }
}

void ChatWindow::contextMenuEvent( QContextMenuEvent* e )
{
    if(_menu)
        _menu->exec(e->globalPos());
}

void ChatWindow::on_pushButton_6_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
    delete _menu;
}

void ChatWindow::delete_friend()
{
    QListWidgetItem * item = ui->listWidget_2->currentItem();
    _friends.erase(item->text());
    int row = ui->listWidget_2->row(item);
    ui->listWidget->takeItem(row);
    chat_message e;
    e.type = chat_message::TABLE;
    e.message = item->text().toStdString();
    e.login = _client.get_login();
    e.list_string.push_back("Delete");
    _client.do_write(e);
    delete item;
}

void ChatWindow::start_message()
{
    QListWidgetItem * item = ui->listWidget_2->currentItem();
    QString name = item->text()+" & "+QString::fromStdString(_client.get_login());
    auto pi = _items.find(name);
    if (pi != _items.end())
    {
        ui->listWidget->setCurrentItem(pi->second);
        auto p = _chats.find(name);
        ui->stackedWidget_2->setCurrentIndex(0);

    }
    else
    {
        QString _name = name;
        QString _participants = item->text();
        emit choose_room(std::string("new"), _name.toStdString(), _participants.toStdString());
    }
    ui->stackedWidget->setCurrentWidget(_chats.find(name)->second);
}

QStringList ChatWindow::get_friends() const
{
    QStringList friendslist;
    for (const auto & x: _friends)
        friendslist << x;
    return friendslist;
}
