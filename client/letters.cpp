#include "letters.h"
#include "ui_letters.h"

Letters::Letters(QString group, ChatWindow * par, QWidget *parent) :
    _group(group),
    _par(par),
    QWidget(parent, Qt::FramelessWindowHint),
    ui(new Ui::Letters)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

   //ui->pushButton->setStyleSheet("#pushButton:pressed{ background-image: url(send_2.png);}");
   //ui->pushButton_2->setStyleSheet("#pushButton:pressed{ background-image: url(file_2.png);}");
   // ui->pushButton->setStyleSheet("#pushButton:pressed{ background-image: url(send_2.png);");
   // ui->pushButton_2->setStyleSheet("#pushButton:pressed{ background-image: url(file_2.png);");

    ui->label->setText(_group);
}


Letters::~Letters()
{
    delete ui;
}

void Letters::on_pushButton_clicked()
{
    emit send_msg(_group, ui->textEdit->toPlainText());
    publish(std::string("Me"), ui->textEdit->toPlainText().toStdString(), 0);
    ui->textEdit->clear();
}

void Letters::publish(std::string lgn,std::string msg, int from)
{
    if ((from == 1) || (from == 0))
    {
        QString message = QString::fromStdString(msg);
        QString login = QString::fromStdString(lgn);
        QDateTime dt = QDateTime::currentDateTime();
        QString text = login+"\n"+message+"\n"+dt.toString("d MMM hh:mm");
        QTextEdit * txt = new QTextEdit;
        txt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txt->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        int height = QFontMetrics(txt->font()).lineSpacing();
        int width = QFontMetrics(txt->font()).maxWidth(); //ширина одного символа
        txt->setFixedWidth(305);
        txt->setFixedHeight((4*height+30)+(message.length()/22)*height*0.75);
        txt->setReadOnly(true);
        QSize as(10, 0);
        if (from == 1)
        {
            QPixmap maskPix( ":/images/mask_2.png" );
            txt->setMask(maskPix.scaled(txt->size()+as).mask());
            txt->setStyleSheet("background-color: qlineargradient(spread:pad, x1:1, y1:0.419, x2:0, y2:0.824, stop:0.0397727 rgba(255, 255, 255, 212), stop:0.318182 rgba(217, 226, 229, 214), stop:0.465909 rgba(179, 214, 198, 215), stop:0.721591 rgba(99, 174, 139, 162), stop:1 rgba(39, 50, 54, 165)); border: 10px, solid, black; border-color: rgb(0, 0, 0);");
            QString text;
            text ="<html><head/><body><p align=\"right\"><span style=\" font-size:12pt;\"><br />" + login+"<br />"+message+"<br />"+dt.toString("d MMM hh:mm");
            text = text + "</span></p></body></html>";
            txt->setHtml(text);
        }
        if (from == 0)
        {
            QString text ="\n" + login+"\n"+message+dt.toString("d MMM hh:mm");
            QPixmap maskPix( ":/images/mask.png" );
            txt->setMask(maskPix.scaled(txt->size()+as).mask());
            txt->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.211818, y1:0.209, x2:1, y2:1, stop:0.0397727 rgba(255, 255, 255, 212), stop:0.318182 rgba(217, 226, 229, 214), stop:0.465909 rgba(179, 201, 214, 215), stop:0.721591 rgba(99, 142, 174, 162), stop:1 rgba(39, 50, 54, 165)); border: 10px, solid, black; border-color: rgb(0, 0, 0);");
            txt->setAlignment(Qt::AlignLeft);
            txt->setPlainText(text);
        }
        QWidget * wgt = new QWidget;
        wgt->setStyleSheet("border: 0px;");
        QLayout * l = new QHBoxLayout;
        l->addWidget(txt);
        if (from)
            l->setAlignment(txt, Qt::AlignLeft);
        else
            l->setAlignment(txt, Qt::AlignRight);

        wgt->setLayout(l);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        as = QSize(0,15);
        item->setSizeHint(txt->size()+as);
        ui->listWidget->setItemWidget(item, wgt);
    }
    if (from == 10)
    {
        QString message = QString::fromStdString(lgn+msg);
        QLabel * lndt = new QLabel;
        lndt->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:12pt;\">"+message+"</span></p></body></html>");
        lndt->setStyleSheet("background-color: qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0 rgba(255, 255, 255, 0), stop:0.373991 rgba(33, 30, 255, 0), stop:0.624043 rgba(255, 0, 0, 0), stop:1 rgba(255, 0, 0, 0)); font: 12pt \"Linux Libertine Display G\";");
        lndt->setMaximumSize(QSize(611, 30));
        lndt->setMinimumSize(QSize(611, 30));
        QWidget * wgt = new QWidget;
        wgt->setStyleSheet("border: 0px;");
        QLayout * l = new QHBoxLayout;
        l->addWidget(lndt);
        wgt->setLayout(l);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(lndt->size());
        ui->listWidget->setItemWidget(item, wgt);
    }
}

void Letters::on_pushButton_5_clicked()
{
    emit group(_group, "delete");
}

void Letters::on_pushButton_4_clicked()
{
    emit group(_group, "exit");
}

void Letters::on_pushButton_3_clicked()
{
    emit participants(_group);
}

void Letters::display_particiants(std::list<std::string> names)
{
    ui->listWidget_2->clear();
    names.pop_front();
    for (auto & x : names)
    {
        makePartItem(ui->listWidget_2, QString::fromStdString(x));
    }
    ui->stackedWidget->setCurrentIndex(1);
}

void Letters::makePartItem( QListWidget* lstWgt, QString sname)
{
    QListWidgetItem* listItem = new QListWidgetItem(sname.section(',',1));
    if (sname.at(0) == '1')
        listItem->setIcon( QPixmap(":/images/online.png") );
    if (sname.at(0) == '0')
        listItem->setIcon( QPixmap(":/images/offline.png") );
    listItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    lstWgt->addItem( listItem );
}

void Letters::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Letters::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    QStringList wordList = _par->get_friends();
    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit->setCompleter(completer);
}

void Letters::on_lineEdit_returnPressed()
{
    emit add_to_group(ui->lineEdit->text(), _group);
    ui->lineEdit->clear();
}

void Letters::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
