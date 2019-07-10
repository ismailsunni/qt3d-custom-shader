#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QTechnique>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QTextureImage>
#include <QUrl>
#include <QSize>

#include "billboardmaterial.h"

BillboardMaterial::BillboardMaterial()
    : mSize( new Qt3DRender::QParameter( "BB_SIZE", QSize(100, 100), this ) )
    , mWindowSize( new Qt3DRender::QParameter( "WIN_SCALE", QSize(800, 600), this ) )
    , mTxt( new Qt3DRender::QParameter( "tex0", "txt", this ) )
{
    addParameter( mSize );
    addParameter( mWindowSize );
    addParameter( mTxt );

    // Shader program
    Qt3DRender::QShaderProgram *shaderProgram = new Qt3DRender::QShaderProgram( this );
    shaderProgram->setVertexShaderCode( Qt3DRender::QShaderProgram::loadSource( QUrl( QStringLiteral( "qrc:/shaders/billboards.vert" ) ) ) );
    shaderProgram->setFragmentShaderCode( Qt3DRender::QShaderProgram::loadSource( QUrl( QStringLiteral( "qrc:/shaders/billboards.frag" ) ) ) );
    shaderProgram->setGeometryShaderCode( Qt3DRender::QShaderProgram::loadSource( QUrl( QStringLiteral( "qrc:/shaders/billboards.geom" ) ) ) );

    // Render Pass
    Qt3DRender::QRenderPass *renderPass = new Qt3DRender::QRenderPass( this );
    renderPass->setShaderProgram(shaderProgram);

    // Technique
    Qt3DRender::QTechnique *technique = new Qt3DRender::QTechnique;
    technique->addRenderPass(renderPass);
    technique->graphicsApiFilter()->setApi( Qt3DRender::QGraphicsApiFilter::OpenGL );
    technique->graphicsApiFilter()->setProfile( Qt3DRender::QGraphicsApiFilter::CoreProfile );
    technique->graphicsApiFilter()->setMajorVersion( 3 );
    technique->graphicsApiFilter()->setMinorVersion( 1 );

    // Effect
    Qt3DRender::QEffect *effect = new Qt3DRender::QEffect( this );
    effect->addTechnique(technique);

    // Texture Image
    Qt3DRender::QTextureImage *image = new Qt3DRender::QTextureImage;
    image->setSource( QString( "qrc:/shaders/success-kid.png" ) );

    // Texture2D
    Qt3DRender::QTexture2D *texture2D = new Qt3DRender::QTexture2D;
    texture2D->setGenerateMipMaps(false);
    texture2D->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    texture2D->setMinificationFilter(Qt3DRender::QTexture2D::Linear);

    texture2D->addTextureImage(image);

    setTexture(texture2D);
    setEffect( effect );
}
