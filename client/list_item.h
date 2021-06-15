#ifndef LIST_ITEM_H
#define LIST_ITEM_H

#include <QWidget>

namespace Ui {
class list_item;
}

class list_item : public QWidget
{
    Q_OBJECT

public:
    explicit list_item(QString name, QWidget *parent = 0);
    QSize size();
    QString get_name(){
        return _name;
    }

    ~list_item();

private:
    Ui::list_item *ui;

    QString _name;
};

#endif // LIST_ITEM_H
