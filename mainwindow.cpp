#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_confirm, &QPushButton::clicked, this, &MainWindow::make_elbow_model);
    connect(ui->pushButton_ImportPart, &QPushButton::clicked, this, &MainWindow::import_part);
    connect(ui->pushButton_extractFace, &QPushButton::clicked, this, &MainWindow::extractFace);
    connect(ui->pushButton_CenterLine, &QPushButton::clicked,this, &MainWindow::onExtractCenterlineButtonClicked);
    connect(ui->pushButton_Mesh1, &QPushButton::clicked,this, &MainWindow::on_meshButton_clicked);

    //过程可视化
    connect(ui->pushButton_initmodel, &QPushButton::clicked,this, &MainWindow::init_model);
    connect(ui->pushButton_meshmodel, &QPushButton::clicked,this, &MainWindow::mesh_model);
    connect(ui->pushButton_surfacemodel, &QPushButton::clicked,this, &MainWindow::surface_model);
    connect(ui->pushButton_centerlinemodel, &QPushButton::clicked,this, &MainWindow::centerline_model);

    //结果可视化
    // 获取QTabWidget的QTabBar对象
    QTabBar *tabBar_init = ui->tabWidget->tabBar();
    // 连接tabBar的tabBarClicked信号到自定义槽函数
    connect(tabBar_init, &QTabBar::tabBarClicked, this, &MainWindow::onTabInitClicked);
    //连接各种结果显示按钮
    connect(ui->pushButton_S, &QPushButton::clicked,this, &MainWindow::onButtonSClicked);
    connect(ui->pushButton_S_Mises, &QPushButton::clicked,this, &MainWindow::onButtonSMisesClicked);
    connect(ui->pushButton_S_principal, &QPushButton::clicked,this, &MainWindow::onButtonSPrincipalClicked);
    connect(ui->pushButton_U, &QPushButton::clicked,this, &MainWindow::onButtonUClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//-----------参数化建模模块----------|
// 添加必要的VTK头文件
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

// OpenCASCADE头文件
// 在包含 OpenCASCADE 头文件之前，抑制弃用警告
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_Triangle.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
// 在包含完 OpenCASCADE 头文件之后，恢复警告设置
#pragma GCC diagnostic pop


// 辅助函数,将OCC形状转换为VTK PolyData
vtkSmartPointer<vtkPolyData> MainWindow::ConvertOCCShapeToVTKPolyData(const TopoDS_Shape& shape, double linearDeflection = 0.5) {
    // 生成三角网格
    BRepMesh_IncrementalMesh mesh(shape, linearDeflection, false, 0.1, true);
    mesh.Perform();

    if (!mesh.IsDone()) {
        throw std::runtime_error("网格生成失败！");
    }

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
    int pointIdOffset = 0;

    // 遍历所有面并提取三角网格
    for (TopExp_Explorer faceExp(shape, TopAbs_FACE); faceExp.More(); faceExp.Next()) {
        TopoDS_Face face = TopoDS::Face(faceExp.Current());
        TopLoc_Location loc;
        Handle(Poly_Triangulation) triFace = BRep_Tool::Triangulation(face, loc);

        if (!triFace.IsNull()) {
            // 添加顶点
            for (Standard_Integer i = 1; i <= triFace->NbNodes(); i++) {
                gp_Pnt p = triFace->Node(i);
                if (!loc.IsIdentity()) {
                    p.Transform(loc.Transformation());
                }
                points->InsertNextPoint(p.X(), p.Y(), p.Z());
            }

            // 添加三角形
            for (Standard_Integer i = 1; i <= triFace->NbTriangles(); i++) {
                Poly_Triangle tri = triFace->Triangle(i);
                Standard_Integer n1, n2, n3;
                tri.Get(n1, n2, n3);

                vtkIdType ids[3] = {
                    n1 - 1 + pointIdOffset,
                    n2 - 1 + pointIdOffset,
                    n3 - 1 + pointIdOffset
                };
                triangles->InsertNextCell(3, ids);
            }

            pointIdOffset += triFace->NbNodes();
        }
    }

    // 构建VTK PolyData
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetPolys(triangles);

    return polyData;
}

// 修改CreateVTKActor函数，移除网格显示，使用实体颜色
vtkSmartPointer<vtkActor> MainWindow::CreateVTKActor(vtkSmartPointer<vtkPolyData> polyData,
                                                     double r, double g, double b) {
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    // 设置实体颜色
    actor->GetProperty()->SetColor(r, g, b);
    actor->GetProperty()->SetOpacity(1.0); // 完全不透明

    // 关闭网格显示
    actor->GetProperty()->EdgeVisibilityOff();

    // 设置光照和材质属性，使实体看起来更自然
    actor->GetProperty()->LightingOn();
    actor->GetProperty()->SetInterpolationToPhong();
    actor->GetProperty()->SetSpecular(0.4);     // 增加高光强度
    actor->GetProperty()->SetSpecularPower(30); // 高光范围
    actor->GetProperty()->SetAmbient(0.2);      // 环境光
    actor->GetProperty()->SetDiffuse(0.8);      // 漫反射

    return actor;
}

//建模函数
void MainWindow::MakeElbowModel(
    double tube_outer_radius, double tube_inner_radius, double tube_length,
    double rotary_sleeve_thickness, double rotary_sleeve_length, double rotary_sleeve_pos,
    double fixed_sleeve_thickness, double fixed_sleeve_length, double fixed_sleeve_pos,
    double arc_radius, double arc_thickness, double arc_angle_rad) {

    const double tolerance = 1E-3;
    const double mesh_precision = 1.0;

    try {
        // 检查目标区域是否存在
        if (!ui->mdiArea) {
            QMessageBox::warning(this, "警告", "显示区域未找到！");
            return;
        }

        // 清理区域中的现有内容
        QLayout* layout = ui->mdiArea->layout();
        if (layout) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0)) != nullptr) {
                if (item->widget()) {
                    item->widget()->setParent(nullptr);
                }
                delete item;
            }
        } else {
            // 如果没有布局，创建新的布局
            layout = new QVBoxLayout(ui->mdiArea);
            ui->mdiArea->setLayout(layout);
        }

        // Tube (管体) 参数
        struct tube_param {
            double R_out, R_in, length;
        } tp = {tube_outer_radius, tube_inner_radius, tube_length};

        // Rotary Sleeve (旋转套) 参数
        struct rotary_sleeve_param {
            double thickness, length, position;
            double R_out, R_in, ref[3], rot[3];
        } rsp = {rotary_sleeve_thickness, rotary_sleeve_length, rotary_sleeve_pos, 0.0, 0.0, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};

        // Fixed Sleeve (固定套) 参数
        struct fixed_sleeve_param {
            double thickness, length, position;
            double R_out, R_in, ref[3], rot[3];
        } fsp = {fixed_sleeve_thickness, fixed_sleeve_length, fixed_sleeve_pos, 0.0, 0.0, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};

        // Semi-circular Sleeve (半圆弧套) 参数
        struct semi_circular_sleeve_param {
            double R, thickness, angle, position, R_out, R_in, ref[3], rot[3];
        } scsp = {arc_radius, arc_thickness, arc_angle_rad, 0.0, 0.0, 0.0, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};

        rsp.position = tp.length - rsp.position;
        rsp.R_in     = tp.R_out + tolerance;
        rsp.R_out    = rsp.R_in + rsp.thickness;

        rsp.ref[0] = rsp.position + rsp.length/2;

        fsp.position = tp.length - fsp.position;
        fsp.R_in     = tp.R_out + tolerance;
        fsp.R_out    = rsp.R_in + rsp.thickness;

        // ref node
        fsp.ref[0] = fsp.position + fsp.length / 2;
        fsp.rot[0] = fsp.position + fsp.length / 2;

        // generate arc
        scsp.position = rsp.position - tolerance;
        scsp.R_in     = tp.R_out + tolerance;
        scsp.R_out    = scsp.R_in + scsp.thickness;

        // rot node
        scsp.rot[0] = scsp.position;
        scsp.rot[1] = -scsp.R;
        std::copy(scsp.rot, scsp.rot+3, scsp.ref);
        std::copy(scsp.rot, scsp.rot+3, rsp.rot);

        // ========== 写入 rigidbody.info ==========
        //路径问题解决
        QString appDirPath = QCoreApplication::applicationDirPath();
        QDir dir(appDirPath);
        dir.cdUp();
        dir.cdUp();
        QString parentDirPath = dir.absolutePath();
        qDebug() << "" << parentDirPath;

        QString filename = parentDirPath + "/Profile/rigidbody.info";
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "---\n";
            out << "Volume2:\n";

            out << "  ref: (" << scsp.ref[0] << "," << scsp.ref[1] << "," << scsp.ref[2] << ")\n";
            out << "  rot: (" << scsp.rot[0] << "," << scsp.rot[1] << "," << scsp.rot[2] << ")\n";

            out << "Volume3:\n";
            out << "  ref: (" << rsp.ref[0] << "," << rsp.ref[1] << "," << rsp.ref[2] << ")\n";
            out << "  rot: (" << rsp.rot[0] << "," << rsp.rot[1] << "," << rsp.rot[2] << ")\n";

            out << "Volume4:\n";
            out << "  ref: (" << fsp.ref[0] << "," << fsp.ref[1] << "," << fsp.ref[2] << ")\n";
            out << "  rot: (" << fsp.rot[0] << "," << fsp.rot[1] << "," << fsp.rot[2] << ")\n";
            file.close();
            qDebug() << "参考点写入 rigidbody.info 成功。";
        } else {
            qDebug() << "无法写入 rigidbody.info 文件！";
        }

        // 创建VTK渲染部件
        QVTKOpenGLNativeWidget *vtkWidget = new QVTKOpenGLNativeWidget(ui->mdiArea);
        vtkWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 创建渲染器
        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->SetBackground(1,1,1); // 背景

        // 创建渲染窗口
        vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow =
            vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        vtkWidget->setRenderWindow(renderWindow);
        renderWindow->AddRenderer(renderer);

        qDebug() << "开始创建弯管模型...";

        // 定义统一的颜色（金属灰色）
        double metal_gray_r = 0.7;
        double metal_gray_g = 0.7;
        double metal_gray_b = 0.75;

        // 1. 创建管体
        qDebug() << "创建管体...";
        gp_Ax2 tube_axis(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0));
        TopoDS_Shape outer_cylinder = BRepPrimAPI_MakeCylinder(tube_axis, tube_outer_radius, tube_length);
        TopoDS_Shape inner_cylinder = BRepPrimAPI_MakeCylinder(tube_axis, tube_inner_radius, tube_length);
        TopoDS_Shape tube = BRepAlgoAPI_Cut(outer_cylinder, inner_cylinder);

        vtkSmartPointer<vtkPolyData> tubePolyData = ConvertOCCShapeToVTKPolyData(tube, mesh_precision);
        vtkSmartPointer<vtkActor> tubeActor = CreateVTKActor(tubePolyData, metal_gray_r, metal_gray_g, metal_gray_b);
        renderer->AddActor(tubeActor);

        // 2. 创建旋转套筒
        qDebug() << "创建旋转套筒...";
        double rotary_pos_from_left = tube_length - rotary_sleeve_pos;
        double rotary_inner_radius = tube_outer_radius + tolerance;
        double rotary_outer_radius = rotary_inner_radius + rotary_sleeve_thickness;

        gp_Ax2 rotary_axis(gp_Pnt(rotary_pos_from_left, 0, 0), gp_Dir(1, 0, 0));
        TopoDS_Shape rotary_inner = BRepPrimAPI_MakeCylinder(rotary_axis, rotary_inner_radius, rotary_sleeve_length);
        TopoDS_Shape rotary_outer = BRepPrimAPI_MakeCylinder(rotary_axis, rotary_outer_radius, rotary_sleeve_length);
        TopoDS_Shape rotary_sleeve = BRepAlgoAPI_Cut(rotary_outer, rotary_inner);

        vtkSmartPointer<vtkPolyData> rotaryPolyData = ConvertOCCShapeToVTKPolyData(rotary_sleeve, mesh_precision);
        vtkSmartPointer<vtkActor> rotaryActor = CreateVTKActor(rotaryPolyData, metal_gray_r, metal_gray_g, metal_gray_b);
        renderer->AddActor(rotaryActor);

        // 3. 创建固定套筒
        qDebug() << "创建固定套筒...";
        double fixed_pos_from_left = tube_length - fixed_sleeve_pos;
        double fixed_inner_radius = tube_outer_radius + tolerance;
        double fixed_outer_radius = fixed_inner_radius + fixed_sleeve_thickness;

        gp_Ax2 fixed_axis(gp_Pnt(fixed_pos_from_left, 0, 0), gp_Dir(1, 0, 0));
        TopoDS_Shape fixed_inner = BRepPrimAPI_MakeCylinder(fixed_axis, fixed_inner_radius, fixed_sleeve_length);
        TopoDS_Shape fixed_outer = BRepPrimAPI_MakeCylinder(fixed_axis, fixed_outer_radius, fixed_sleeve_length);
        TopoDS_Shape fixed_sleeve = BRepAlgoAPI_Cut(fixed_outer, fixed_inner);

        vtkSmartPointer<vtkPolyData> fixedPolyData = ConvertOCCShapeToVTKPolyData(fixed_sleeve, mesh_precision);
        vtkSmartPointer<vtkActor> fixedActor = CreateVTKActor(fixedPolyData, metal_gray_r, metal_gray_g, metal_gray_b);
        renderer->AddActor(fixedActor);

        // 4. 创建圆弧段
        qDebug() << "创建圆弧段...";
        double arc_position = rotary_pos_from_left - tolerance;
        double arc_inner_radius = tube_outer_radius + tolerance;
        double arc_outer_radius = arc_inner_radius + arc_thickness;

        // 创建圆弧截面
        Handle(Geom_TrimmedCurve) inner_arc = GC_MakeArcOfCircle(
            gp_Pnt(arc_position, 0, -arc_inner_radius),
            gp_Pnt(arc_position, -arc_inner_radius, 0),
            gp_Pnt(arc_position, 0, arc_inner_radius)
            );

        Handle(Geom_TrimmedCurve) outer_arc = GC_MakeArcOfCircle(
            gp_Pnt(arc_position, 0, -arc_outer_radius),
            gp_Pnt(arc_position, -arc_outer_radius, 0),
            gp_Pnt(arc_position, 0, arc_outer_radius)
            );

        // 创建连接边
        TopoDS_Edge left_edge = BRepBuilderAPI_MakeEdge(
            gp_Pnt(arc_position, 0, -arc_inner_radius),
            gp_Pnt(arc_position, 0, -arc_outer_radius)
            );

        TopoDS_Edge right_edge = BRepBuilderAPI_MakeEdge(
            gp_Pnt(arc_position, 0, arc_inner_radius),
            gp_Pnt(arc_position, 0, arc_outer_radius)
            );

        // 创建截面线框
        BRepBuilderAPI_MakeWire wire_builder;
        wire_builder.Add(BRepBuilderAPI_MakeEdge(outer_arc));
        wire_builder.Add(right_edge);
        wire_builder.Add(BRepBuilderAPI_MakeEdge(inner_arc));
        wire_builder.Add(left_edge);
        TopoDS_Wire section_wire = wire_builder.Wire();

        // 创建截面面
        TopoDS_Face section_face = BRepBuilderAPI_MakeFace(section_wire);

        // 旋转生成圆弧段
        gp_Ax1 rotation_axis(
            gp_Pnt(arc_position, -arc_radius, 0),
            gp_Dir(0, 0, 1)
            );
        TopoDS_Shape arc_sector = BRepPrimAPI_MakeRevol(section_face, rotation_axis, arc_angle_rad);

        vtkSmartPointer<vtkPolyData> arcPolyData = ConvertOCCShapeToVTKPolyData(arc_sector, mesh_precision * 0.5);
        vtkSmartPointer<vtkActor> arcActor = CreateVTKActor(arcPolyData, metal_gray_r, metal_gray_g, metal_gray_b);
        renderer->AddActor(arcActor);

        // 5. 写入参考点信息

        // 6. 设置相机并渲染
        renderer->ResetCamera();

        // 7. 将VTK部件添加到布局中
        layout->addWidget(vtkWidget);

        // 8. 强制刷新布局和渲染
        ui->mdiArea->layout()->activate();
        renderWindow->Render();

        // 9. 添加交互器
        vtkSmartPointer<vtkRenderWindowInteractor> interactor = renderWindow->GetInteractor();
        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
            vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
        interactor->SetInteractorStyle(style);

        qDebug() << "模型创建完成，已全屏显示在指定区域中";
    } catch (const std::exception& e) {
        qDebug() << "错误:" << e.what();
        QMessageBox::critical(this, "错误", QString("模型创建失败: %1").arg(e.what()));
    }
}

//显示函数
void MainWindow::ShowModelInMdiArea(vtkSmartPointer<vtkRenderer> renderer) {
    try {
        // 清空MDI区域的所有子窗口
        ui->mdiArea->closeAllSubWindows();

        // 创建VTK部件
        QVTKOpenGLNativeWidget *vtkWidget = new QVTKOpenGLNativeWidget();
        vtkWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 创建渲染窗口
        vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow =
            vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        vtkWidget->setRenderWindow(renderWindow);
        renderWindow->AddRenderer(renderer);

        // 创建全屏子窗口（无边框）
        QMdiSubWindow *subWindow = new QMdiSubWindow(ui->mdiArea);
        subWindow->setWidget(vtkWidget);
        subWindow->setWindowFlags(Qt::FramelessWindowHint); // 无边框
        subWindow->setAttribute(Qt::WA_DeleteOnClose);

        // 添加到MDI区域并最大化
        ui->mdiArea->addSubWindow(subWindow);
        subWindow->showMaximized();

        // 渲染
        renderWindow->Render();

        // 设置交互器
        vtkSmartPointer<vtkRenderWindowInteractor> interactor = renderWindow->GetInteractor();
        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
            vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
        interactor->SetInteractorStyle(style);

        qDebug() << "模型已全屏显示在MDI区域中";

    } catch (const std::exception& e) {
        qDebug() << "显示模型时出错:" << e.what();
        QMessageBox::critical(this, "错误", QString("显示模型失败: %1").arg(e.what()));
    }
}

//实现函数
void MainWindow::make_elbow_model() {
    bool ok[12];
    double tube_outer_radius        = ui->lineEdit_Rout->text().toDouble(&ok[0]);
    double tube_inner_radius         = ui->lineEdit_Rin->text().toDouble(&ok[1]);
    double tube_length       = ui->lineEdit_Length->text().toDouble(&ok[2]);
    double rotary_sleeve_thickness     = ui->lineEdit_SleeveThickness1->text().toDouble(&ok[3]);
    double rotary_sleeve_length     = ui->lineEdit_SleeveLength1->text().toDouble(&ok[4]);
    double fixed_sleeve_thickness     = ui->lineEdit_SleeveThickness->text().toDouble(&ok[5]);
    double fixed_sleeve_length     = ui->lineEdit_SleeveLength->text().toDouble(&ok[6]);
    double rotary_sleeve_pos   = ui->lineEdit_RotaryPos->text().toDouble(&ok[7]);
    double fixed_sleeve_pos    = ui->lineEdit_FixedPos->text().toDouble(&ok[8]);
    double arc_radius        = ui->lineEdit_ArcR->text().toDouble(&ok[9]);
    double arc_thickness        = ui->lineEdit_ArcThickness->text().toDouble(&ok[10]);
    double arc_angle_rad    = ui->lineEdit_ArcAngle->text().toDouble(&ok[11]);

    MakeElbowModel(
        tube_outer_radius, tube_inner_radius, tube_length,
        rotary_sleeve_thickness, rotary_sleeve_length, rotary_sleeve_pos,
        fixed_sleeve_thickness, fixed_sleeve_length, fixed_sleeve_pos,
        arc_radius, arc_thickness, arc_angle_rad
        );
}

//----------数模显示----------|
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <TopoDS_Shape.hxx>
#include <BRepTools.hxx>
#include <STEPControl_Reader.hxx>
#include <Interface_Static.hxx>
#include <TCollection_HAsciiString.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangulation.hxx>
#include <IGESControl_Reader.hxx>
#include <vtkCallbackCommand.h>
#pragma GCC diagnostic pop

//读取STP文件
TopoDS_Shape MainWindow::ReadSTEPFile(const QString& fileName)
{
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(fileName.toStdString().c_str());

    if (status != IFSelect_RetDone) {
        qWarning() << "读取STEP文件失败:" << fileName;
        return TopoDS_Shape();
    }

    // 将所有可转换的形状加载到模型
    reader.TransferRoots();
    TopoDS_Shape shape = reader.OneShape();

    if (shape.IsNull()) {
        qWarning() << "转换失败：文件中无有效几何体";
        return TopoDS_Shape();
    }

    return shape;
}

//读取IGS文件
TopoDS_Shape MainWindow::ReadIGESFile(const QString& fileName)
{
    IGESControl_Reader reader;

    IFSelect_ReturnStatus status = reader.ReadFile(fileName.toStdString().c_str());

    if (status != IFSelect_RetDone) {
        qWarning() << "读取IGES文件失败:" << fileName;
        return TopoDS_Shape();
    }

    // 设置精度模式
    Interface_Static::SetCVal("read.precision.mode", "1"); // 启用精度设置
    Interface_Static::SetRVal("read.precision.val", 1.0e-6);

    // 传输所有根实体
    reader.TransferRoots();

    // 获取合并后的整体形状
    TopoDS_Shape shape = reader.OneShape();

    if (shape.IsNull()) {
        qWarning() << "转换失败：IGES文件中无有效几何体";
        return TopoDS_Shape();
    }

    return shape;
}

//复用显示逻辑
void MainWindow::DisplayShape(const TopoDS_Shape& shape)
{
    try {
        if (shape.IsNull()) {
            QMessageBox::warning(this, "警告", "无效的几何体，无法显示！");
            return;
        }

        // 清理 ui->mdiArea 中的旧内容
        QLayout* layout = ui->mdiArea->layout();
        if (layout) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0)) != nullptr) {
                if (item->widget()) {
                    item->widget()->setParent(nullptr);
                }
                delete item;
            }
        } else {
            layout = new QVBoxLayout(ui->mdiArea);
            ui->mdiArea->setLayout(layout);
        }

        // 创建 VTK 渲染部件
        QVTKOpenGLNativeWidget *vtkWidget = new QVTKOpenGLNativeWidget(ui->mdiArea);
        vtkWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 创建渲染器
        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->SetBackground(1, 1, 1); // 白色背景

        // 创建渲染窗口
        vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow =
            vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        vtkWidget->setRenderWindow(renderWindow);
        renderWindow->AddRenderer(renderer);

        // 转换 OCC Shape 为 VTK PolyData
        BRepMesh_IncrementalMesh mesh(shape, 0.01);
        mesh.Perform();

        if (!mesh.IsDone()) {
            throw std::runtime_error("网格生成失败！");
        }

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
        int pointIdOffset = 0;

        // 清空旧的映射
        m_faceMap.clear();

        // 准备建立新映射
        std::vector<TopoDS_Face> faceOrderList; // 按顺序存储所有 Face
        TopExp_Explorer tempExp(shape, TopAbs_FACE);
        for (; tempExp.More(); tempExp.Next()) {
            faceOrderList.push_back(TopoDS::Face(tempExp.Current()));
        }

        int currentVtkCellId = 0; // VTK 中 Cell 的全局 ID

        // 修改后的循环（带映射）
        int faceIndex = 0;
        for (const auto& face : faceOrderList) {
            TopLoc_Location loc;
            Handle(Poly_Triangulation) triFace = BRep_Tool::Triangulation(face, loc);

            if (!triFace.IsNull()) {
                int numNodes = triFace->NbNodes();
                int numTriangles = triFace->NbTriangles();

                // 添加顶点
                for (int i = 1; i <= numNodes; i++) {
                    gp_Pnt p = triFace->Node(i);
                    if (!loc.IsIdentity()) {
                        p.Transform(loc.Transformation());
                    }
                    points->InsertNextPoint(p.X(), p.Y(), p.Z());
                }

                // 添加三角形 & 建立映射
                for (int i = 1; i <= numTriangles; i++) {
                    Poly_Triangle tri = triFace->Triangle(i);
                    Standard_Integer n1, n2, n3;
                    tri.Get(n1, n2, n3);

                    vtkIdType ids[3] = {n1-1+pointIdOffset, n2-1+pointIdOffset, n3-1+pointIdOffset};
                    vtkIdType cellId = triangles->InsertNextCell(3, ids);

                    // 记录映射关系
                    m_faceMap[currentVtkCellId + i - 1] = face;
                }

                pointIdOffset += numNodes;
                currentVtkCellId += numTriangles; // 更新 Cell ID
            }
            faceIndex++;
        }

        // 创建 PolyData
        vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points);
        polyData->SetPolys(triangles);

        // 创建 Actor
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(polyData);

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor(0.8, 0.8, 0.8);     // 灰色
        actor->GetProperty()->SetOpacity(1.0);
        actor->GetProperty()->EdgeVisibilityOff();
        actor->GetProperty()->SetInterpolationToPhong();

        // 添加到渲染器
        renderer->AddActor(actor);
        renderer->ResetCamera();

        // 添加到布局
        layout->addWidget(vtkWidget);
        renderWindow->Render();

        // 设置交互器
        vtkSmartPointer<vtkRenderWindowInteractor> interactor = renderWindow->GetInteractor();
        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
            vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
        interactor->SetInteractorStyle(style);

        // 创建回调命令对象
        vtkSmartPointer<vtkCallbackCommand> clickCallback =
            vtkSmartPointer<vtkCallbackCommand>::New();
        clickCallback->SetCallback(OnLeftButtonDown);
        clickCallback->SetClientData(this); // 传递 this 指针

        // 添加观察者
        interactor->AddObserver(vtkCommand::LeftButtonPressEvent, clickCallback);

        qDebug() << "模型已成功显示。";
        qDebug() << "模型类型：" << m_currentShape.ShapeType();

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("显示模型失败: %1").arg(e.what()));
    }
}

//显示数模
void MainWindow::import_part()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "打开数模文件",
        QDir::homePath(),
        "交换格式 (*.stp *.step *.igs *.iges);;STEP 文件 (*.stp *.step);;IGES 文件 (*.igs *.iges);;所有文件 (*)"
        );

    if (fileName.isEmpty()) return;

    TopoDS_Shape shape;
    QString suffix = QFileInfo(fileName).suffix().toLower();

    if (suffix == "stp" || suffix == "step") {
        shape = ReadSTEPFile(fileName);
    }
    else if (suffix == "igs" || suffix == "iges") {
        shape = ReadIGESFile(fileName);
    }
    else {
        QMessageBox::warning(this, "不支持", "仅支持 .stp/.step/.igs/.iges");
        return;
    }

    if (!shape.IsNull()) {
        m_currentShape = shape;        // 👈 保存当前模型
        DisplayShape(shape);
    } else {
        QMessageBox::warning(this, "错误", "无法读取或解析该文件！");
    }
}

//----------提取外表面----------|
#include <vtkRendererCollection.h>
#include <unordered_set>
#include <BRepAdaptor_Surface.hxx>
#include <STEPControl_Writer.hxx>

//鼠标点击事件响应函数
void MainWindow::OnLeftButtonDown(vtkObject* obj, unsigned long eid, void* clientdata, void* calldata)
{
    MainWindow* self = static_cast<MainWindow*>(clientdata);
    vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(obj);
    int* pos = interactor->GetEventPosition();

    vtkRenderer* renderer = self->ui->mdiArea->findChild<QVTKOpenGLNativeWidget*>()->renderWindow()->GetRenderers()->GetFirstRenderer();

    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.0005);
    picker->Pick(pos[0], pos[1], 0, renderer);

    if (picker->GetCellId() != -1) {
        vtkIdType cellId = picker->GetCellId();
        auto it = self->m_faceMap.find(cellId);
        if (it != self->m_faceMap.end()) {
            TopoDS_Face clickedFace = it->second;
            TopoDS_Shape outerSurface = self->FindConnectedOuterSurface(self->m_currentShape, clickedFace);
            if (!outerSurface.IsNull()) {
                self->m_extractedOuterSurface = outerSurface; //保存结果
                self->DisplayShape(outerSurface);
            }
        }
    }
}

//BFS 算法提取相连的圆柱/环面/B样条面，组成外壳
TopoDS_Shape MainWindow::FindConnectedOuterSurface(const TopoDS_Shape& shape, const TopoDS_Face& seedFace)
{
    // 1. 判断种子面是否是圆柱面、环面或 B-Spline 面
    BRepAdaptor_Surface surf(seedFace);
    GeomAbs_SurfaceType type = surf.GetType();

    // 支持三种类型：圆柱、环面、B样条（自由曲面）
    if (type != GeomAbs_Cylinder && type != GeomAbs_Torus && type != GeomAbs_BSplineSurface) {
        qDebug() << "【警告】点击的面不是圆柱/环面/B样条面，无法提取外壁";
        qDebug() << "         面类型代码：" << type; // 打印类型编号，便于调试
        return TopoDS_Shape();
    }

    qDebug() << "【调试】种子面类型：" << type;

    // 2. 初始化 BFS
    BRep_Builder builder;
    TopoDS_Compound result;
    builder.MakeCompound(result);

    std::unordered_set<TopoDS_Shape, ShapeHash, ShapeEqual> visited;
    std::queue<TopoDS_Face> toVisit;
    toVisit.push(seedFace);

    int processedCount = 0;
    while (!toVisit.empty()) {
        TopoDS_Face currentFace = toVisit.front();
        toVisit.pop();

        if (visited.count(currentFace) > 0) continue;
        visited.insert(currentFace);
        builder.Add(result, currentFace);
        processedCount++;

        // 3. 查找相邻面
        TopExp_Explorer edgeExp(currentFace, TopAbs_EDGE);
        for (; edgeExp.More(); edgeExp.Next()) {
            TopoDS_Edge edge = TopoDS::Edge(edgeExp.Current());
            TopTools_ListOfShape faceList;
            GetFacesSharingEdge(shape, edge, faceList);

            for (const auto& adjFaceShape : faceList) {
                TopoDS_Face adjFace = TopoDS::Face(adjFaceShape);
                if (visited.count(adjFace) > 0) continue;

                // 4. 判断相邻面是否是圆柱/环面/B样条
                BRepAdaptor_Surface adjSurf(adjFace);
                GeomAbs_SurfaceType adjType = adjSurf.GetType();
                if (adjType != GeomAbs_Cylinder && adjType != GeomAbs_Torus && adjType != GeomAbs_BSplineSurface) {
                    continue; // 不是这三种类型，跳过
                }

                toVisit.push(adjFace);
            }
        }
    }

    qDebug() << "【提取外壁】成功提取" << processedCount << "个面";
    return result;
}

//判断两条边是否相同，几何上重合
bool AreEdgesSame(const TopoDS_Edge& e1, const TopoDS_Edge& e2)
{
    if (e1.IsNull() || e2.IsNull()) return false;

    // 正确声明参数变量
    Standard_Real first1, last1;
    Handle(Geom_Curve) curve1 = BRep_Tool::Curve(e1, first1, last1);

    Standard_Real first2, last2;
    Handle(Geom_Curve) curve2 = BRep_Tool::Curve(e2, first2, last2);

    // 检查曲线是否有效
    if (curve1.IsNull() || curve2.IsNull()) {
        return false;
    }

    // 检查曲线类型是否相同
    if (curve1->DynamicType() != curve2->DynamicType()) {
        return false;
    }

    // 比较端点位置
    gp_Pnt p1_start = BRep_Tool::Pnt(TopExp::FirstVertex(e1, Standard_True));
    gp_Pnt p1_end   = BRep_Tool::Pnt(TopExp::LastVertex(e1, Standard_True));
    gp_Pnt p2_start = BRep_Tool::Pnt(TopExp::FirstVertex(e2, Standard_True));
    gp_Pnt p2_end   = BRep_Tool::Pnt(TopExp::LastVertex(e2, Standard_True));

    Standard_Real tol = Precision::Confusion();
    bool sameOrder   = (p1_start.Distance(p2_start) < tol && p1_end.Distance(p2_end) < tol);
    bool reverseOrder = (p1_start.Distance(p2_end) < tol && p1_end.Distance(p2_start) < tol);

    return sameOrder || reverseOrder;
}

//查找与给定边共享的所有面。
void MainWindow::GetFacesSharingEdge(const TopoDS_Shape& shape, const TopoDS_Edge& edge, TopTools_ListOfShape& faceList)
{
    TopTools_MapOfShape uniqueFaces;
    TopExp_Explorer exp(shape, TopAbs_FACE);
    for (; exp.More(); exp.Next()) {
        TopoDS_Face face = TopoDS::Face(exp.Current());
        TopExp_Explorer edgeExp(face, TopAbs_EDGE);
        for (; edgeExp.More(); edgeExp.Next()) {
            TopoDS_Edge currentEdge = TopoDS::Edge(edgeExp.Current());
            // 使用自定义函数判断边是否相同
            if (AreEdgesSame(currentEdge, edge)) {
                if (!uniqueFaces.Contains(face)) {
                    uniqueFaces.Add(face);
                    faceList.Append(face);
                }
                break;
            }
        }
    }
}

//保存外壁
void MainWindow::extractFace()
{
    if (m_extractedOuterSurface.IsNull()) {
        QMessageBox::warning(this, "警告", "没有提取到外壁模型！");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("保存外壁模型"), "", tr("STEP 文件 (*.stp *.step)"));

    if (fileName.isEmpty()) {
        return; // 用户取消保存
    }

    // 确保文件名以 .stp 或 .step 结尾
    if (!fileName.endsWith(".stp", Qt::CaseInsensitive) &&
        !fileName.endsWith(".step", Qt::CaseInsensitive)) {
        fileName += ".stp";
    }

    STEPControl_Writer writer;
    // 使用 STEPControl_Writer 更简单
    STEPControl_Writer stepWriter;

    // 设置 STEP 标准
    Interface_Static::SetCVal("write.step.schema", "AP214");

    // 写入模型
    stepWriter.Transfer(m_extractedOuterSurface, STEPControl_AsIs);
    IFSelect_ReturnStatus status = stepWriter.Write(fileName.toStdString().c_str());

    if (status == IFSelect_RetDone) {
        QMessageBox::information(this, "成功", "外壁模型已保存为: " + fileName);
    } else {
        QMessageBox::critical(this, "错误", "保存失败！");
    }
}


//----------提取中心线段----------|
#include <QFileDialog>
#include <QMessageBox>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Geom_Line.hxx>

// OpenCASCADE 头文件
#include <BRepAdaptor_Surface.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Lin.hxx>
#include <gp_Circ.hxx>
#include <STEPControl_Writer.hxx>
#include <Interface_Static.hxx>
#include <BRepLib.hxx>
#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx> // 如果需要对边进行网格化
#include <vtkPolyLine.h>

// 显示形状（支持颜色和线宽）
#include <vtkPolyLine.h>

void MainWindow::DisplayOuterSurfaceAndCenterline(const TopoDS_Shape& outerShape, const TopoDS_Shape& centerlineShape)
{
    try {
        if (outerShape.IsNull() || centerlineShape.IsNull()) {
            QMessageBox::warning(this, "警告", "无效的几何体，无法显示！");
            return;
        }

        // 清理 ui->mdiArea 中的旧内容
        QLayout* layout = ui->mdiArea->layout();
        if (layout) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0)) != nullptr) {
                if (item->widget()) {
                    delete item->widget();
                }
                delete item;
            }
        } else {
            layout = new QVBoxLayout(ui->mdiArea);
            ui->mdiArea->setLayout(layout);
        }

        // 创建 VTK 渲染部件
        QVTKOpenGLNativeWidget *vtkWidget = new QVTKOpenGLNativeWidget(ui->mdiArea);
        vtkWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 创建渲染器
        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->SetBackground(0.1, 0.1, 0.1);

        // 创建渲染窗口
        vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow =
            vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        vtkWidget->setRenderWindow(renderWindow);
        renderWindow->AddRenderer(renderer);

        // 1. 显示外壁模型 (半透明)
        {
            BRepMesh_IncrementalMesh mesh(outerShape, 0.01); // 网格精度
            mesh.Perform();
            if (!mesh.IsDone()) {
                throw std::runtime_error("外壁模型网格生成失败！");
            }

            vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
            vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
            vtkIdType pointIdOffset = 0;

            TopExp_Explorer exp(outerShape, TopAbs_FACE);
            for (; exp.More(); exp.Next()) {
                TopoDS_Face face = TopoDS::Face(exp.Current());
                TopLoc_Location loc;
                Handle(Poly_Triangulation) triFace = BRep_Tool::Triangulation(face, loc);

                if (!triFace.IsNull()) {
                    int numNodes = triFace->NbNodes();
                    int numTriangles = triFace->NbTriangles();

                    for (int i = 1; i <= numNodes; i++) {
                        gp_Pnt p = triFace->Node(i);
                        if (!loc.IsIdentity()) {
                            p.Transform(loc.Transformation());
                        }
                        points->InsertNextPoint(p.X(), p.Y(), p.Z());
                    }

                    for (int i = 1; i <= numTriangles; i++) {
                        Poly_Triangle tri = triFace->Triangle(i);
                        Standard_Integer n1, n2, n3;
                        tri.Get(n1, n2, n3);

                        vtkIdType ids[3] = {n1-1+pointIdOffset, n2-1+pointIdOffset, n3-1+pointIdOffset};
                        triangles->InsertNextCell(3, ids);
                    }
                    pointIdOffset += numNodes;
                }
            }

            vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
            polyData->SetPoints(points);
            polyData->SetPolys(triangles);

            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputData(polyData);

            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            actor->SetMapper(mapper);
            actor->GetProperty()->SetColor(0.8, 0.8, 0.8); // 灰色
            actor->GetProperty()->SetOpacity(0.5);         // 半透明
            //actor->GetProperty()->SetEdgeVisibilityOff();
            actor->GetProperty()->SetInterpolationToPhong();

            renderer->AddActor(actor);
        }

        // 2. 显示中心线 (红色)
        {
            vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
            vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
            vtkIdType pointIdCounter = 0;

            TopExp_Explorer exp(centerlineShape, TopAbs_EDGE);
            for (; exp.More(); exp.Next()) {
                TopoDS_Edge edge = TopoDS::Edge(exp.Current());
                if (edge.IsNull()) continue;

                Standard_Real first, last;
                Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
                if (curve.IsNull()) continue;

                const int numPoints = 50; // 用于离散化曲线的点数
                vtkSmartPointer<vtkPolyLine> polyline = vtkSmartPointer<vtkPolyLine>::New();
                polyline->GetPointIds()->SetNumberOfIds(numPoints);

                for (int i = 0; i < numPoints; ++i) {
                    Standard_Real u = first + (last - first) * i / (numPoints - 1);
                    gp_Pnt p = curve->Value(u);
                    points->InsertNextPoint(p.X(), p.Y(), p.Z());
                    polyline->GetPointIds()->SetId(i, pointIdCounter++);
                }
                lines->InsertNextCell(polyline);
            }

            vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
            polyData->SetPoints(points);
            polyData->SetLines(lines);

            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputData(polyData);

            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            actor->SetMapper(mapper);
            actor->GetProperty()->SetColor(1.0, 0.0, 0.0); // 红色
            actor->GetProperty()->SetLineWidth(3.0);       // 线宽

            renderer->AddActor(actor);
        }

        // 3. 渲染
        renderer->ResetCamera(); // 自动调整相机视角

        layout->addWidget(vtkWidget);
        renderWindow->Render();

        // 设置交互器
        vtkSmartPointer<vtkRenderWindowInteractor> interactor = renderWindow->GetInteractor();
        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
            vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
        interactor->SetInteractorStyle(style);
        interactor->Initialize(); // 初始化交互器
        // interactor->Start();      // 对于嵌入式部件，这通常由 Qt 管理

        qDebug() << "外壁模型和中心线已成功显示。";

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("显示模型失败: %1").arg(e.what()));
    }
}

// 提取中心线
#include <BRepLib.hxx> // 确保包含这个头文件
#include <Geom_Line.hxx> // 如果使用替代方案
#include <Geom_Circle.hxx> //用于处理圆弧
#include <GeomAdaptor_Curve.hxx> // 用于适配曲线

TopoDS_Shape MainWindow::ExtractAnalyticalCenterlines(const TopoDS_Shape& shape)
{
    qDebug() << "ExtractAnalyticalCenterlines 开始处理 shape type:" << shape.ShapeType();

    BRep_Builder builder;
    TopoDS_Compound comp;
    builder.MakeCompound(comp);

    TopExp_Explorer exp(shape, TopAbs_FACE);
    int faceCount = 0;
    for (; exp.More(); exp.Next()) {
        faceCount++;
        TopoDS_Face face = TopoDS::Face(exp.Current());

        if (face.IsNull()) {
            qDebug() << "  跳过空面";
            continue;
        }

        BRepAdaptor_Surface surf(face);
        GeomAbs_SurfaceType surfType = surf.GetType();

        qDebug() << "  处理第" << faceCount << "个面, 类型:" << surfType;

        if (surfType != GeomAbs_Cylinder && surfType != GeomAbs_Torus) {
            continue;
        }

        if (surfType == GeomAbs_Cylinder) {
            qDebug() << "    -> 圆柱面";
            try {
                gp_Ax1 axis = surf.Cylinder().Axis();
                Handle(Geom_Line) line = new Geom_Line(axis);

                // 在圆柱面处理部分
                TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(line, -500, 500);
                if (!edge.IsNull()) {
                    // ✅ 添加几何验证
                    BRepLib::BuildCurves3d(edge);
                    Standard_Real first, last;
                    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
                    if (curve.IsNull()) {
                        qDebug() << "    -> Edge 几何无效，跳过";
                    } else {
                        builder.Add(comp, edge);
                        qDebug() << "    -> 成功提取轴线";
                    }
                }

            } catch (...) {
                qDebug() << "    -> 提取圆柱轴线时发生异常";
            }
        }
        else if (surfType == GeomAbs_Torus) {
            qDebug() << "    -> 环面";
            try {
                gp_Torus torus = surf.Torus();
                gp_Ax1 axis = torus.Axis();
                gp_Circ centerCircle(gp_Ax2(axis.Location(), axis.Direction()), torus.MajorRadius());

                // 在环面处理部分
                TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(centerCircle);
                if (!edge.IsNull()) {
                    // 添加几何验证
                    BRepLib::BuildCurves3d(edge);
                    Standard_Real first, last;
                    Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
                    if (curve.IsNull()) {
                        qDebug() << "    -> Edge 几何无效，跳过";
                    } else {
                        builder.Add(comp, edge);
                        qDebug() << "    -> 成功提取中心圆";
                    }
                }

            } catch (...) {
                qDebug() << "    -> 提取环面中心圆时发生异常";
            }
        }
    }

    qDebug() << "ExtractAnalyticalCenterlines 处理完成，共处理" << faceCount << "个面";
    return comp;
}

// 中心线实现按钮
void MainWindow::onExtractCenterlineButtonClicked()
{
    qDebug() << "=== 开始提取中心线 ===";

    if (m_extractedOuterSurface.IsNull()) {
        qDebug() << "警告: m_extractedOuterSurface 为空";
        QMessageBox::warning(this, "警告", "没有提取到外壁模型！");
        return;
    }

    qDebug() << "Outer surface type:" << m_extractedOuterSurface.ShapeType();

    qDebug() << "调用 ExtractAnalyticalCenterlines...";
    TopoDS_Shape centerlines = ExtractAnalyticalCenterlines(m_extractedOuterSurface);
    qDebug() << "ExtractAnalyticalCenterlines 返回";

    if (centerlines.IsNull()) {
        qDebug() << "警告: 未提取到中心线";
        QMessageBox::warning(this, "警告", "未找到任何圆柱/环面，无法提取中心线！");
        return;
    }

    qDebug() << "保存中心线结果";
    m_extractedCenterline = centerlines;

    qDebug() << "显示外壁模型(透明)和中心线(红色)";
    // 调用新的显示函数，传入外壁和中心线
    DisplayOuterSurfaceAndCenterline(m_extractedOuterSurface, centerlines);

    qDebug() << "=== 中心线提取及显示完成 ===";
}

//----------网格划分-----------|
#include <BRepBuilderAPI_Copy.hxx>

//网格模型显示
void MainWindow::DisplayMeshedShape(const TopoDS_Shape& meshedShape)
{
    try {
        if (meshedShape.IsNull()) {
            QMessageBox::warning(this, tr("警告"), tr("无效的几何体，无法显示！"));
            qDebug() << "DisplayMeshedShape: 传入的 meshedShape 为空。";
            return;
        }

        // --- 1. 清理 ui->mdiArea 中的旧内容 ---
        QLayout* layout = ui->mdiArea->layout();
        if (layout) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0)) != nullptr) {
                if (item->widget()) {
                    item->widget()->setParent(nullptr);
                    delete item->widget();
                }
                delete item;
            }
        } else {
            layout = new QVBoxLayout(ui->mdiArea);
            ui->mdiArea->setLayout(layout);
        }

        // --- 2. 创建 VTK 渲染部件 ---
        QVTKOpenGLNativeWidget *vtkWidget = new QVTKOpenGLNativeWidget(ui->mdiArea);
        vtkWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // --- 3. 创建渲染器和渲染窗口 ---
        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->SetBackground(1, 1, 1);

        vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow =
            vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        vtkWidget->setRenderWindow(renderWindow);
        renderWindow->AddRenderer(renderer);

        // --- 4. 从已划分的 Shape 提取 VTK PolyData ---
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
        int pointIdOffset = 0;
        int totalPointsAdded = 0;
        int totalCellsAdded = 0;

        m_faceMap.clear(); // 清空旧的面映射
        std::vector<TopoDS_Face> faceOrderList;
        TopExp_Explorer tempExp(meshedShape, TopAbs_FACE);
        int faceCount = 0;
        for (; tempExp.More(); tempExp.Next()) {
            faceOrderList.push_back(TopoDS::Face(tempExp.Current()));
            faceCount++;
        }
        qDebug() << "DisplayMeshedShape: 找到 " << faceCount << " 个面。";

        int currentVtkCellId = 0;
        for (const auto& face : faceOrderList) {
            TopLoc_Location loc;
            Handle(Poly_Triangulation) triFace = BRep_Tool::Triangulation(face, loc);

            if (!triFace.IsNull()) {
                int numNodes = triFace->NbNodes();
                int numTriangles = triFace->NbTriangles();

                for (int i = 1; i <= numNodes; i++) {
                    gp_Pnt p = triFace->Node(i);
                    if (!loc.IsIdentity()) {
                        p.Transform(loc.Transformation());
                    }
                    points->InsertNextPoint(p.X(), p.Y(), p.Z());
                }
                totalPointsAdded += numNodes;

                for (int i = 1; i <= numTriangles; i++) {
                    Poly_Triangle tri = triFace->Triangle(i);
                    Standard_Integer n1, n2, n3;
                    tri.Get(n1, n2, n3);

                    vtkIdType ids[3] = {n1 - 1 + pointIdOffset, n2 - 1 + pointIdOffset, n3 - 1 + pointIdOffset};
                    vtkIdType cellId = triangles->InsertNextCell(3, ids);

                    m_faceMap[currentVtkCellId + i - 1] = face;
                }
                totalCellsAdded += numTriangles;
                pointIdOffset += numNodes;
                currentVtkCellId += numTriangles;
            }
        }

        qDebug() << "DisplayMeshedShape: 总共添加了 " << totalPointsAdded << " 个顶点, "
                 << totalCellsAdded << " 个三角形。";

        if (totalCellsAdded == 0) {
            QString warnMsg = tr("模型似乎没有有效的网格数据（0 个三角形），无法显示。");
            qDebug() << "DisplayMeshedShape: " << warnMsg;
            QMessageBox::warning(this, tr("警告"), warnMsg);
            delete vtkWidget;
            return;
        }

        // --- 5. 创建 VTK PolyData 对象 ---
        vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
        polyData->SetPoints(points);
        polyData->SetPolys(triangles);

        // --- 6. 创建 Mapper 和 Actor ---
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(polyData);

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        // --- 7. 设置透明度和网格线显示 ---
        // 设置面颜色
        actor->GetProperty()->SetColor(0.8, 0.8, 0.8); // 灰色
        // 设置不透明度 (0.0 完全透明, 1.0 完全不透明)
        actor->GetProperty()->SetOpacity(0.3); // 例如，30% 不透明度
        // 开启边缘/网格线显示
        actor->GetProperty()->EdgeVisibilityOn();
        // 设置边缘/网格线颜色 (例如，黑色)
        actor->GetProperty()->SetEdgeColor(0, 0, 0);
        // 设置表面表示为表面 (SURFACE) 或线框 (WIREFRAME)
        // WIREFRAME 只显示网格线，SURFACE 显示面和线
        // 如果只想显示线，用 WIREFRAME；如果想显示透明面+线，用 SURFACE
        actor->GetProperty()->SetRepresentationToSurface();

        // --- 8. 添加 Actor 到渲染器并重置相机 ---
        renderer->AddActor(actor);
        renderer->ResetCamera();

        // --- 9. 将 VTK 部件添加到 UI 布局 ---
        layout->addWidget(vtkWidget);

        // --- 10. 触发渲染 ---
        renderWindow->Render();

        // --- 11. 设置交互器和样式 ---
        vtkSmartPointer<vtkRenderWindowInteractor> interactor = renderWindow->GetInteractor();
        if (interactor) {
            vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
                vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
            interactor->SetInteractorStyle(style);
        } else {
            qWarning() << "DisplayMeshedShape: 获取 VTK 渲染窗口交互器失败。";
        }

        qDebug() << "DisplayMeshedShape: 透明网格模型显示成功。";
    } catch (const std::exception& e) {
        QString errorMsg = QString("DisplayMeshedShape: 显示透明网格模型失败: %1").arg(e.what());
        QMessageBox::critical(this, tr("错误"), errorMsg);
        qDebug() << errorMsg;
    } catch (...) {
        QString errorMsg = QString("DisplayMeshedShape: 显示透明网格模型时发生未知异常。");
        QMessageBox::critical(this, tr("错误"), errorMsg);
        qDebug() << errorMsg;
    }
}

//网格划分功能
void MainWindow::on_meshButton_clicked()
{
    // 1. 检查是否有原始模型加载
    if (m_currentShape.IsNull()) {
        QMessageBox::warning(this, tr("网格划分"), tr("请先加载一个模型！"));
        qDebug() << "网格划分失败：未加载原始模型。";
        return;
    }

    // 2. 从界面获取网格大小参数
    double linearDeflection = ui->spinBox->value();
    // 确保值是正数
    if (linearDeflection <= 0.0) {
        QMessageBox::warning(this, tr("网格划分"), tr("网格大小必须大于 0！"));
        qDebug() << "网格划分失败：网格大小无效 (" << linearDeflection << ")。";
        return;
    }

    // 3. 创建原始模型的深拷贝，避免修改 m_currentShape 本身
    // BRepMesh_IncrementalMesh 会修改 Shape 内部的三角剖分数据
    BRepBuilderAPI_Copy copier(m_currentShape);
    TopoDS_Shape shapeToMesh = copier.Shape();

    if (shapeToMesh.IsNull()) {
        QString errorMsg = QString("创建模型副本失败！");
        QMessageBox::critical(this, tr("网格划分错误"), errorMsg);
        qDebug() << errorMsg;
        return;
    }

    // 4. 对副本执行网格划分
    try {
        // BRepMesh_IncrementalMesh 构造函数会直接修改传入的 shapeToMesh
        BRepMesh_IncrementalMesh mesher(shapeToMesh, linearDeflection, Standard_False, 0.5);

        // 执行网格划分
        mesher.Perform();

        // 检查是否成功
        if (!mesher.IsDone()) {
            QString errorMsg = QString("网格划分失败！OpenCASCADE Mesher 返回 IsDone() 为 false。");
            QMessageBox::critical(this, tr("网格划分错误"), errorMsg);
            qDebug() << errorMsg;
            return;
        }

        // 5. 将划分后的模型存储到新成员变量 ---
        m_meshedShape = shapeToMesh;
        qDebug() << "划分后的模型已存储到 m_meshedShape。";

        // 6. 显示划分后的模型
        DisplayMeshedShape(m_meshedShape);
        QMessageBox::information(this, tr("网格划分"), tr("网格划分完成，新模型已存储并显示！"));
    } catch (const std::exception& e) {
        QString errorMsg = QString("网格划分过程中发生异常: %1").arg(e.what());
        QMessageBox::critical(this, tr("网格划分错误"), errorMsg);
        qDebug() << errorMsg;
        // 发生错误时，清空存储的划分后模型
        m_meshedShape = TopoDS_Shape();
    } catch (...) {
        QString errorMsg = QString("网格划分过程中发生未知异常。");
        QMessageBox::critical(this, tr("网格划分错误"), errorMsg);
        qDebug() << errorMsg;
        // 发生错误时，清空存储的划分后模型
        m_meshedShape = TopoDS_Shape();
    }
}


//----------过程可视化----------|
//原始模型
void MainWindow::init_model(){
    if (!m_currentShape.IsNull()) {
        DisplayShape(m_currentShape);
    } else {
        QMessageBox::warning(this, "错误", "没有可用模型！");
    }
}
//网格模型
void MainWindow::mesh_model(){
    if (!m_meshedShape.IsNull()) {
        DisplayMeshedShape(m_meshedShape);
    } else {
        QMessageBox::warning(this, "错误", "没有可用模型！");
    }
}
//表面模型
void MainWindow::surface_model(){
    if (!m_extractedOuterSurface.IsNull()) {
        DisplayShape(m_extractedOuterSurface);
    } else {
        QMessageBox::warning(this, "错误", "没有可用模型！");
    }
}
//中线模型
void MainWindow::centerline_model(){

}


//----------结果可视化----------|
#include <vtkTextProperty.h>

//tab栏读取VTK文件
void MainWindow::onTabInitClicked(int index)
{
    // 获取被点击的tab的文本
    QString tabText = ui->tabWidget->tabText(index);

    // 根据点击的tab索引执行不同的逻辑
    switch (index) {
    case 0:
        qDebug() << "第一个标签页被点击了! 索引:" << index << ", 文本:" << tabText;
        // 在这里添加第一个tab的点击处理逻辑
        break;
    case 1:
        qDebug() << "第二个标签页被点击了! 索引:" << index << ", 文本:" << tabText;
        // 在这里添加第二个tab的点击处理逻辑
        break;
    case 2:
        qDebug() << "第三个标签页被点击了! 索引:" << index << ", 文本:" << tabText;
        // 在这里添加第三个tab的点击处理逻辑
        break;
    case 3:
        qDebug() << "第四个标签页被点击了! 索引:" << index << ", 文本:" << tabText;
        {
            // 1. 获取当前工作目录
            QString currentDir = QDir::currentPath();
            // 2. 构建results文件夹路径
            QString resultsDirPath = currentDir + "/results";
            QDir resultsDir(resultsDirPath);

            // 3. 检查results文件夹是否存在
            if (!resultsDir.exists()) {
                qWarning() << "Results directory does not exist: " << resultsDirPath;
                return;
            }

            // 4. 获取results文件夹下所有以.vtk结尾的文件
            QStringList nameFilters;
            nameFilters << "*.vtk";
            QFileInfoList fileInfoList = resultsDir.entryInfoList(nameFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name); // 按名称排序

            // 5. 提取文件路径到QStringList
            for (const QFileInfo& fileInfo : fileInfoList) {
                vtkFilePaths.append(fileInfo.absoluteFilePath());
            }
            qDebug() << "Found " << vtkFilePaths.size() << " VTK files in " << resultsDirPath;
        }
        break;
    default:
        qDebug() << "未知的标签页被点击! 索引:" << index;
        break;
    }
}

void MainWindow::VisualVTKGroupFile(const QStringList& fileNames, const QString& scalarType)
{
    // --- 1. 清空并准备 MDI 子窗口 ---
    QLayout* layout_vtk = ui->mdiArea->layout();
    if (layout_vtk) {
        QLayoutItem* item;
        while ((item = layout_vtk->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->setParent(nullptr);
                delete item->widget();
            }
            delete item;
        }
    } else {
        layout_vtk = new QVBoxLayout(ui->mdiArea);
        ui->mdiArea->setLayout(layout_vtk);
    }

    // 检查是否有文件需要处理
    if (fileNames.isEmpty()) {
        qDebug() << "No files provided to VisualVTKGroupFile.";
        return; // 如果没有文件，直接返回
    }

    // 创建一个中心部件用于容纳 QVTKOpenGLNativeWidget
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    // 创建 QVTKOpenGLNativeWidget
    QVTKOpenGLNativeWidget *vtkWidget = new QVTKOpenGLNativeWidget(centralWidget);

    // 创建 VTK 渲染窗口和渲染器
    // 每次调用函数时都创建新的RenderWindow和Renderer
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> localRenderWindow =
        vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> localRenderer = vtkSmartPointer<vtkRenderer>::New();
    localRenderer->SetBackground(1.0, 1.0, 1.0);
    localRenderWindow->AddRenderer(localRenderer);

    // 设置 VTK 部件的渲染窗口
    vtkWidget->setRenderWindow(localRenderWindow);

    // 将 VTK 部件添加到布局
    layout->addWidget(vtkWidget);

    // 创建全屏子窗口
    QMdiSubWindow *subWindow = new QMdiSubWindow(ui->mdiArea);
    subWindow->setWidget(centralWidget); // 设置包含VTK部件的中心部件
    subWindow->setWindowFlags(Qt::FramelessWindowHint); // 无边框
    subWindow->setAttribute(Qt::WA_DeleteOnClose);

    // 将子窗口添加到 MDI 区域
    ui->mdiArea->addSubWindow(subWindow);

    // --- 2. 执行 VTK 可视化逻辑 ---
    QString currentTypeForThisWindow = scalarType;
    int currentFrame = 0;
    QList<vtkActor*> vtkActorsForThisWindow;
    QList<vtkUnstructuredGrid*> vtkGridsForThisWindow;
    vtkSmartPointer<vtkUnstructuredGridReader> reader;

    // 在循环外创建scalarBar，但只在需要时
    vtkSmartPointer<vtkScalarBarActor> scalarBar = nullptr; // 初始化为nullptr
    vtkSmartPointer<vtkLookupTable> lut = nullptr; // 也推迟LUT的创建

    for (const auto& fileName : fileNames) {
        reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
        reader->SetFileName(fileName.toStdString().c_str());
        reader->Update();

        vtkSmartPointer<vtkDataArray> selectedScalar;
        QString color_name = "Solid";
        bool useScalar = true;

        std::string typeStr = currentTypeForThisWindow.toStdString();
        selectedScalar = reader->GetOutput()->GetPointData()->GetArray(typeStr.c_str());
        color_name = currentTypeForThisWindow.toStdString().c_str();

        if (!selectedScalar) {
            qWarning() << "标量数组 " << currentTypeForThisWindow << " 不存在 in file " << fileName << "!";
            continue; // 跳过当前文件，继续处理下一个
        }

        vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputConnection(reader->GetOutputPort());

        if (useScalar && selectedScalar) {
            int numComponents = selectedScalar->GetNumberOfComponents();
            mapper->SetScalarVisibility(true);
            mapper->SelectColorArray(selectedScalar->GetName());

            if (numComponents == 1) {
                mapper->SetScalarModeToUsePointFieldData();
            } else {
                mapper->SetScalarModeToUsePointFieldData();
            }

            double range[2];
            selectedScalar->GetRange(range);
            if (range[0] >= range[1]) {
                range[0] = 0;
                range[1] = 1;
            }
            mapper->SetScalarRange(range[0], range[1]);

            // --- 在这里创建LUT和scalarBar（如果还没有） ---
            if (!lut) { // 只在第一次找到有效的scalar时创建LUT
                lut = vtkSmartPointer<vtkLookupTable>::New();
                lut->SetHueRange(0.666667, 0.0); // Blue to Red
                lut->Build();
                mapper->SetLookupTable(lut);
            } else { // 如果LUT已存在，也应用到这个mapper
                mapper->SetLookupTable(lut);
            }

            // 在这里创建scalarBar
            if (!scalarBar) { // 只在第一次找到有效的scalar时创建scalarBar
                scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
                scalarBar->SetLookupTable(lut); // 使用刚创建或已存在的LUT
                scalarBar->SetTitle(color_name.toStdString().c_str());
                scalarBar->SetNumberOfLabels(10);
                scalarBar->SetDragable(true);

                // --- 设置标量条文字颜色和固定字体大小 ---
                scalarBar->GetLabelTextProperty()->SetColor(0.0, 0.0, 0.0); // 黑色
                scalarBar->GetLabelTextProperty()->SetFontSize(18); // 固定字体大小
                scalarBar->GetTitleTextProperty()->SetColor(0.0, 0.0, 0.0); // 黑色
                scalarBar->GetTitleTextProperty()->SetFontSize(20); // 固定字体大小

                // 保持颜色条大小固定 (相对窗口大小的比例)
                scalarBar->SetWidth(0.1);
                scalarBar->SetHeight(0.8);

                // 只添加一次scalarBar到renderer
                localRenderer->AddActor2D(scalarBar);
            }

        } else {
            mapper->SetScalarVisibility(false);
        }

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->EdgeVisibilityOn();
        actor->GetProperty()->SetAmbient(0.25);
        if (!useScalar) {
            actor->GetProperty()->SetColor(0.8, 0.8, 0.8);
        }

        vtkGridsForThisWindow.push_back(reader->GetOutput());
        vtkActorsForThisWindow.push_back(actor);
        localRenderer->AddActor(actor);
    }

    // 检查是否成功添加了任何带有标量的Actor（即是否创建了LUT和scalarBar）
    if (!lut) {
        qDebug() << "警告: 没有找到有效的标量数组用于任何输入文件，scalarBar 未创建。";
    }

    currentFrame = 0;

    // --- 3. 设置交互器 ---
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = localRenderWindow->GetInteractor();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle(style);

    // --- 4. 强制刷新布局和渲染 ---
    localRenderWindow->Render();

    // --- 5. 重置相机以适应模型 ---
    localRenderer->ResetCamera();

    // --- 6. 再次渲染以应用相机调整 ---
    localRenderWindow->Render();

    // 显示子窗口并最大化
    subWindow->showMaximized();
}

//显示S
void MainWindow::onButtonSClicked()
{
    // vtkFilePaths是之前获取的VTK文件路径列表
    VisualVTKGroupFile(vtkFilePaths, "S");
}
//显示S_Mises
void MainWindow::onButtonSMisesClicked()
{
    VisualVTKGroupFile(vtkFilePaths, "S_Mises");
}
//显示S_Principal
void MainWindow::onButtonSPrincipalClicked()
{
    VisualVTKGroupFile(vtkFilePaths, "S_Principal");
}
//显示U
void MainWindow::onButtonUClicked()
{
    VisualVTKGroupFile(vtkFilePaths, "U");
}






















