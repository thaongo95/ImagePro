#ifndef STYLE_H
#define STYLE_H

#include <QObject>

class Style : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList toolbarColor READ toolbarColor WRITE setToolbarColor NOTIFY toolbarColorChanged FINAL)
    Q_PROPERTY(QStringList tooltripColor READ tooltripColor WRITE setTooltripColor NOTIFY tooltripColorChanged FINAL)
public:
    explicit Style(QObject *parent = nullptr);
    QStringList toolbarColor() {return _toolBarColor;}
    QStringList tooltripColor() {return _toolTripColor;}
    void setToolbarColor(QStringList value);
    void setTooltripColor(QStringList value);
signals:
    void toolbarColorChanged();
    void tooltripColorChanged();
private:
    QStringList _toolBarColor{"#8fbc8f"};
    QStringList _toolTripColor{"#e9967a"};
};

#endif // STYLE_H
