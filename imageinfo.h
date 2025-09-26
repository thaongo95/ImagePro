#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QObject>

class ImageInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged FINAL)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged FINAL)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged FINAL)

public:
    explicit ImageInfo(QObject *parent = nullptr);

    int height() const {return m_height;}
    int width() const {return m_width;}
    QString imageUrl() const {return m_url;}

    void setHeight(int value);
    void setWidth(int value);
    void setImageUrl(QString value);
signals:
    void heightChanged();
    void widthChanged();
    void imageUrlChanged();
private:
    int m_height;
    int m_width;
    QString m_url;
};

#endif // IMAGEINFO_H
