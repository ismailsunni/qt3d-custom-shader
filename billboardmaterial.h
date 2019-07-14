#ifndef BILLBOARDMATERIAL_H
#define BILLBOARDMATERIAL_H

#include <QObject>
#include <QSize>
#include <Qt3DRender/QMaterial>
#include <Qt3DExtras/QTextureMaterial>


class BillboardMaterial : public Qt3DRender::QMaterial
{

    Q_OBJECT
public:
    BillboardMaterial();

    void setSize(const QSizeF size);
    QSizeF size() const;

    void setWindowSize(const QSizeF size);
    QSizeF windowSize() const;

    void setTexture2DFromImagePath(QString imagePath);

private:
  Qt3DRender::QParameter *mSize = nullptr;
  Qt3DRender::QParameter *mWindowSize = nullptr;
  Qt3DRender::QParameter *mTexture2D = nullptr;
};

#endif // BILLBOARDMATERIAL_H
