#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QTechnique>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QTextureImage>
#include <QUrl>
#include <QSizeF>
#include <QVariant>

#include "billboardmaterial.h"

BillboardMaterial::BillboardMaterial()
    : mSize( new Qt3DRender::QParameter( "BB_SIZE", QSizeF(100, 100), this ) )
    , mWindowSize( new Qt3DRender::QParameter( "WIN_SCALE", QSizeF(800, 600), this ) )
{
    addParameter( mSize );
    addParameter( mWindowSize );

    Qt3DRender::QTextureImage *image = new Qt3DRender::QTextureImage;
    image->setSource(QUrl( QStringLiteral( "qrc:/shaders/success-kid.png") ));

    // Texture2D
    Qt3DRender::QTexture2D *texture2D = new Qt3DRender::QTexture2D;
    texture2D->setGenerateMipMaps(false);
    texture2D->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    texture2D->setMinificationFilter(Qt3DRender::QTexture2D::Linear);

    texture2D->addTextureImage(image);

    mTexture2D = new Qt3DRender::QParameter( "tex0", texture2D, this );

    addParameter(mTexture2D);

    // Shader program
    Qt3DRender::QShaderProgram *shaderProgram = new Qt3DRender::QShaderProgram( this );
    shaderProgram->setVertexShaderCode( Qt3DRender::QShaderProgram::loadSource( QUrl( QStringLiteral( "qrc:/shaders/billboards.vert" ) ) ) );
    shaderProgram->setFragmentShaderCode( Qt3DRender::QShaderProgram::loadSource( QUrl( QStringLiteral( "qrc:/shaders/billboards.frag" ) ) ) );
    shaderProgram->setGeometryShaderCode( Qt3DRender::QShaderProgram::loadSource( QUrl( QStringLiteral( "qrc:/shaders/billboards.geom" ) ) ) );

    // Render Pass
    Qt3DRender::QRenderPass *renderPass = new Qt3DRender::QRenderPass( this );
    renderPass->setShaderProgram(shaderProgram);

    // without this filter the default forward renderer would not render this
    Qt3DRender::QFilterKey *filterKey = new Qt3DRender::QFilterKey;
    filterKey->setName( QStringLiteral( "renderingStyle" ) );
    filterKey->setValue( "forward" );

    // Technique
    Qt3DRender::QTechnique *technique = new Qt3DRender::QTechnique;
    technique->addRenderPass(renderPass);
    technique->addFilterKey(filterKey);
    technique->graphicsApiFilter()->setApi( Qt3DRender::QGraphicsApiFilter::OpenGL );
    technique->graphicsApiFilter()->setProfile( Qt3DRender::QGraphicsApiFilter::CoreProfile );
    technique->graphicsApiFilter()->setMajorVersion( 3 );
    technique->graphicsApiFilter()->setMinorVersion( 1 );

    // Effect
    Qt3DRender::QEffect *effect = new Qt3DRender::QEffect( this );
    effect->addTechnique(technique);

    setEffect( effect );
}

void BillboardMaterial::setSize(const QSizeF size)
{
    mSize->setValue(size);
}

QSizeF BillboardMaterial::size() const
{
    return mSize->value().value<QSizeF>();
}

void BillboardMaterial::setWindowSize(const QSizeF size)
{
    mWindowSize->setValue(size);
}

QSizeF BillboardMaterial::windowSize() const
{
    return mWindowSize->value().value<QSizeF>();
}

void BillboardMaterial::setTexture2D(Qt3DRender::QTexture2D *texture2D)
{
    mTexture2D->setValue(QVariant::fromValue(texture2D));
}

Qt3DRender::QTexture2D* BillboardMaterial::texture2D()
{
    QVariant variant = mTexture2D->value();
    return qvariant_cast<Qt3DRender::QTexture2D*>(variant);
}

void BillboardMaterial::setTexture2DFromImagePath(QString imagePath)
{
    // Texture Image
    Qt3DRender::QTextureImage *image = new Qt3DRender::QTextureImage;
    image->setSource(QUrl( imagePath ));

    // Texture2D
    Qt3DRender::QTexture2D *texture2D = new Qt3DRender::QTexture2D;
    texture2D->setGenerateMipMaps(false);
    texture2D->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    texture2D->setMinificationFilter(Qt3DRender::QTexture2D::Linear);

    texture2D->addTextureImage(image);

    setTexture2D(texture2D);
}
