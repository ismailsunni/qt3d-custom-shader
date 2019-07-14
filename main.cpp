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
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/qorbitcameracontroller.h>

#include <Qt3DRender/QCamera>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QMaterial>
#include <Qt3DExtras/QFirstPersonCameraController>

#include <QtGui/QScreen>

#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QCommandLinkButton>
#include <QPushButton>

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
    QWidget *controlWidget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout;
    QGridLayout *controlLayout = new QGridLayout(controlWidget);
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    vLayout->addWidget(controlWidget);

    // Create control widget
    QCommandLinkButton *info = new QCommandLinkButton();
    info->setText(QStringLiteral("Qt3D Billboard"));
    info->setIconSize(QSize(0,0));

    // Random size
    QPushButton *randomSizeButton = new QPushButton(controlWidget);
    randomSizeButton->setText(QStringLiteral("Random Size"));
    randomSizeButton->setToolTip(QStringLiteral("Random Size"));

    // Bigger billboard size
    QPushButton *biggerSizeButton = new QPushButton(controlWidget);
    biggerSizeButton->setText(QStringLiteral("Make Bigger"));
    biggerSizeButton->setToolTip(QStringLiteral("Make the size of billboard bigger"));

    // Smaller billboard size
    QPushButton *smallerSizeButton = new QPushButton(controlWidget);
    smallerSizeButton->setText(QStringLiteral("Make Smaller"));
    smallerSizeButton->setToolTip(QStringLiteral("Make the size of billboard smaller"));

    // Put to layout
    controlLayout->addWidget(info);
    controlLayout->addWidget(randomSizeButton);
    controlLayout->addWidget(biggerSizeButton);
    controlLayout->addWidget(smallerSizeButton);

    widget->setWindowTitle(QStringLiteral("Custom Shader for Billboard"));

    // Camera
    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 100.0f);
    QVector3D originalPosition(0, 10.0f, 20.0f);
    camera->setPosition(originalPosition);
    QVector3D originalViewCenter(0, 0, 0);
    camera->setViewCenter(originalViewCenter);
    QVector3D upVector(0, 1.0, 0);
    camera->setUpVector(upVector);

    // Camera control
    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
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
    cuboidMaterial->setDiffuse(QColor(255, 0, 0, 255));
    cuboidMaterial->setAmbient(QColor(255, 0, 0, 255));
    cuboidMaterial->setSpecular(QColor(255, 0, 0, 255));
    cuboidMaterial->setAlpha(0.5);
    cuboidMaterial->setShininess(1.0);

    // Cuboid entity
    Qt3DCore::QEntity *cuboidEntity = new Qt3DCore::QEntity(rootEntity);
    cuboidEntity->addComponent(cuboid);
    cuboidEntity->addComponent(cuboidTransform);
    cuboidEntity->addComponent(cuboidMaterial);
    cuboidEntity->setEnabled(true);

    // Blue Plane mesh
    Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh();
    planeMesh->setWidth(20);
    planeMesh->setHeight(20);

    Qt3DExtras::QPhongAlphaMaterial *planeMaterial = new Qt3DExtras::QPhongAlphaMaterial(rootEntity);
    planeMaterial->setAmbient(QColor(0, 0, 255, 255));

    Qt3DCore::QEntity *planeEntity = new Qt3DCore::QEntity(rootEntity);
    planeEntity->addComponent(planeMesh);
    planeEntity->addComponent(planeMaterial);
    planeEntity->setEnabled(true);

    // Green Sphere
    Qt3DExtras::QSphereMesh *sphereMesh = new  Qt3DExtras::QSphereMesh();
    Qt3DExtras::QPhongAlphaMaterial *greenMaterial = new Qt3DExtras::QPhongAlphaMaterial(rootEntity);
    greenMaterial->setAmbient(QColor(0, 255, 0, 255));
    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();
    sphereTransform->setTranslation(QVector3D(0.0f, 5.0f, 0.0f));

    Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(rootEntity);
    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(greenMaterial);
    sphereEntity->addComponent(sphereTransform);
    sphereEntity->setEnabled(true);


    // Billboard
    // Points
    QVector<QVector3D> pos;
    pos << QVector3D(1, 1, 0);
    pos << QVector3D(-1, 2, 8);
    pos << QVector3D(1, 1, 7);
    pos << QVector3D(0, 0, 4);

    // Billboard Geometry
    BillboardGeometry *billboardGeometry = new BillboardGeometry();
    billboardGeometry->setPoints(pos);

    // Billboard Geometry Renderer
    Qt3DRender::QGeometryRenderer *billboardGeometryRenderer = new Qt3DRender::QGeometryRenderer;
    billboardGeometryRenderer->setPrimitiveType( Qt3DRender::QGeometryRenderer::Points );
    billboardGeometryRenderer->setGeometry( billboardGeometry );
    billboardGeometryRenderer->setVertexCount( billboardGeometry->count() );

    // Billboard Material
    BillboardMaterial *billboardMaterial = new BillboardMaterial();
    billboardMaterial->setSize(QSizeF(100, 100));

    // Billboard Transform
    Qt3DCore::QTransform *billboardTransform = new Qt3DCore::QTransform();
    billboardTransform->setTranslation(QVector3D(0.0f, 1.5f, 0.0f));

    // Billboard Entity
    Qt3DCore::QEntity *billboardEntity = new Qt3DCore::QEntity(rootEntity);
    billboardEntity->addComponent(billboardMaterial);
    billboardEntity->addComponent(billboardGeometryRenderer);
    billboardEntity->addComponent(billboardTransform);
    billboardEntity->setEnabled(true);


    // Signal and slot for widgets
    QObject::connect(randomSizeButton, &QPushButton::clicked, rootEntity, [ = ]{
        int randomNumber = (qrand() % (20)) * 10 + 10; // Random number multiple of 10 between 0 to 200, mull
        billboardMaterial->setSize(QSizeF(randomNumber, randomNumber));

    });

    QObject::connect(biggerSizeButton, &QPushButton::clicked, rootEntity, [ = ]{
        billboardMaterial->setSize(billboardMaterial->size() + QSizeF(10, 10));
    });

    QObject::connect(smallerSizeButton, &QPushButton::clicked, rootEntity, [ = ]{
        // Minus size --> reverse the orientation of the image
        billboardMaterial->setSize(billboardMaterial->size() - QSizeF(10, 10));
    });

    view->setRootEntity(rootEntity);

    widget->show();
    widget->resize(1600, 1200);

    return app.exec();
}
