#include "billboardgeometry.h"
#include "billboardmaterial.h"

#include <QColor>
#include <QDebug>
#include <QObject>
#include <QPropertyAnimation>
#include <QThread>
#include <QVector3D>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongAlphaMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/qorbitcameracontroller.h>

#include <Qt3DRender/QCamera>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QMaterial>

#include <QtGui/QScreen>

#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

int main(int argc, char *argv[])
{
    // Application
    QApplication app(argc, argv);

    // Window
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(400, 300));
    container->setMaximumSize(screenSize);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    widget->setWindowTitle(QStringLiteral("Custom Shader for Billboard"));

    // Camera
    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    QVector3D originalPosition(0, 0, 50.0f);
    camera->setPosition(originalPosition);
    QVector3D originalViewCenter(0, 0, 0);
    camera->setViewCenter(originalViewCenter);

    // Camera control
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setLookSpeed(180.0f);
    camController->setLinearSpeed(50.0f);
    camController->setCamera(camera);

    // Cuboid mesh data
    Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();
    cuboid->setXExtent(2);
    cuboid->setYExtent(2);
    cuboid->setZExtent(2);

    // Cuboid mesh transform
    Qt3DCore::QTransform *cuboidTransform = new Qt3DCore::QTransform();
    cuboidTransform->setScale(3.0);
    cuboidTransform->setTranslation(QVector3D(15.0f, 0.0f, 0.0f));

    // Cuboid material
    Qt3DExtras::QPhongAlphaMaterial *cuboidMaterial = new Qt3DExtras::QPhongAlphaMaterial(rootEntity);
    cuboidMaterial->setDiffuse(QColor(0, 0, 255, 255));
    cuboidMaterial->setAmbient(QColor(0, 0, 255, 255));
    cuboidMaterial->setSpecular(QColor(0, 0, 255, 255));
    cuboidMaterial->setAlpha(0.5);
    cuboidMaterial->setShininess(1.0);

    // Cuboid entity
    Qt3DCore::QEntity *cuboidEntity = new Qt3DCore::QEntity(rootEntity);
    cuboidEntity->addComponent(cuboid);
    cuboidEntity->addComponent(cuboidTransform);
    cuboidEntity->addComponent(cuboidMaterial);
    cuboidEntity->setEnabled(true);

    // Billboard
    // Points
    QVector<QVector3D> pos;
    pos << QVector3D(1, 1, 0);
    pos << QVector3D(-1, 2, 8);
    pos << QVector3D(1, 1, 7);
    pos << QVector3D(0, 0, 4);

    // Billboard Geometry
    BillboardGeometry *bbg = new BillboardGeometry();
    bbg->setPoints(pos);

    // Billboard Geometry Renderer
    Qt3DRender::QGeometryRenderer *billboardGeometryRenderer = new Qt3DRender::QGeometryRenderer;
    billboardGeometryRenderer->setPrimitiveType( Qt3DRender::QGeometryRenderer::Points );
    billboardGeometryRenderer->setGeometry( bbg );
    billboardGeometryRenderer->setVertexCount( bbg->count() );

    // Billboard Material
    BillboardMaterial *billboardMaterial = new BillboardMaterial();

    // Billboard Transform
    Qt3DCore::QTransform *billboardTransform = new Qt3DCore::QTransform();
    billboardTransform->setTranslation(QVector3D(0.0f, 1.5f, 0.0f));

    // Billboard Entity
    Qt3DCore::QEntity *billboardEntity = new Qt3DCore::QEntity(rootEntity);
    billboardEntity->addComponent(billboardMaterial);
    billboardEntity->addComponent(billboardGeometryRenderer);
    billboardEntity->addComponent(billboardTransform);
    billboardEntity->setEnabled(true);

    view->setRootEntity(rootEntity);

    widget->show();
    widget->resize(1600, 1200);

    return app.exec();
}
