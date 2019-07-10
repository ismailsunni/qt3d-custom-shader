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

private:
  Qt3DRender::QParameter *mSize = nullptr;
  Qt3DRender::QParameter *mWindowSize = nullptr;
  Qt3DRender::QParameter *mTxt = nullptr;
};

#endif // BILLBOARDMATERIAL_H
