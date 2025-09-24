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
}

MainWindow::~MainWindow()
{
    delete ui;
}

//-----------参数化建模模块----------

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


// 辅助函数：将OCC形状转换为VTK PolyData
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
        //WriteReferencePointsToFile(rotary_pos_from_left, fixed_pos_from_left, arc_position, arc_radius);

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
// 实现函数
void MainWindow::make_elbow_model() {
    qDebug() << "弯管模型创建...";

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

//----------数模显示----------

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

//读取文件
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

TopoDS_Shape MainWindow::ReadIGESFile(const QString& fileName)
{
    IGESControl_Reader reader;

    IFSelect_ReturnStatus status = reader.ReadFile(fileName.toStdString().c_str());

    if (status != IFSelect_RetDone) {
        qWarning() << "读取IGES文件失败:" << fileName;
        return TopoDS_Shape();
    }

    // 设置精度模式（可选）
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

        // ================== 转换 OCC Shape 为 VTK PolyData ==================
        // double linearDeflection = 0.05; // 精度，越小越光滑
        // double angularDeflection = 0.5; // 角度偏差（弧度）

        // BRepMesh_IncrementalMesh mesh(shape, linearDeflection, Standard_True, angularDeflection);
        BRepMesh_IncrementalMesh mesh(shape, 0.01);
        mesh.Perform();

        if (!mesh.IsDone()) {
            throw std::runtime_error("网格生成失败！");
        }

        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
        int pointIdOffset = 0;

        // ✅ 【D.1】清空旧的映射
        m_faceMap.clear();

        // ✅ 【D.2】准备建立新映射
        std::vector<TopoDS_Face> faceOrderList; // 按顺序存储所有 Face
        TopExp_Explorer tempExp(shape, TopAbs_FACE);
        for (; tempExp.More(); tempExp.Next()) {
            faceOrderList.push_back(TopoDS::Face(tempExp.Current()));
        }

        int currentVtkCellId = 0; // VTK 中 Cell 的全局 ID

        // ✅ 【D.3】修改后的循环（带映射）
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

                    // ✅ 【D.4】记录映射关系
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

        // ✅ 【E】创建回调命令对象
        vtkSmartPointer<vtkCallbackCommand> clickCallback =
            vtkSmartPointer<vtkCallbackCommand>::New();
        clickCallback->SetCallback(OnLeftButtonDown);
        clickCallback->SetClientData(this); // 传递 this 指针

        // ✅ 添加观察者
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

//----------提取外表面----------
#include <vtkRendererCollection.h>
#include <unordered_set>
#include <BRepAdaptor_Surface.hxx>

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
                self->DisplayShape(outerSurface);
            }
        }
    }
}

//BFS 算法提取相连的圆柱/环面/B样条面，组成外壳
TopoDS_Shape MainWindow::FindConnectedOuterSurface(const TopoDS_Shape& shape, const TopoDS_Face& seedFace)
{
    // ✅ 1. 判断种子面是否是圆柱面、环面或 B-Spline 面
    BRepAdaptor_Surface surf(seedFace);
    GeomAbs_SurfaceType type = surf.GetType();

    // ✅ 支持三种类型：圆柱、环面、B样条（自由曲面）
    if (type != GeomAbs_Cylinder && type != GeomAbs_Torus && type != GeomAbs_BSplineSurface) {
        qDebug() << "【警告】点击的面不是圆柱/环面/B样条面，无法提取外壁";
        qDebug() << "         面类型代码：" << type; // 打印类型编号，便于调试
        return TopoDS_Shape(); // 返回空
    }

    qDebug() << "【调试】种子面类型：" << type;

    // ✅ 2. 初始化 BFS
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

        // ✅ 3. 查找相邻面
        TopExp_Explorer edgeExp(currentFace, TopAbs_EDGE);
        for (; edgeExp.More(); edgeExp.Next()) {
            TopoDS_Edge edge = TopoDS::Edge(edgeExp.Current());
            TopTools_ListOfShape faceList;
            GetFacesSharingEdge(shape, edge, faceList);

            for (const auto& adjFaceShape : faceList) {
                TopoDS_Face adjFace = TopoDS::Face(adjFaceShape);
                if (visited.count(adjFace) > 0) continue;

                // ✅ 4. 判断相邻面是否是圆柱/环面/B样条
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

//判断两条边是否相同（几何上重合）
bool AreEdgesSame(const TopoDS_Edge& e1, const TopoDS_Edge& e2)
{
    if (e1.IsNull() || e2.IsNull()) return false;

    // ✅ 正确声明参数变量
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

    // 可选：进一步比较曲线几何（如类型为圆、直线等）
    // 这里可以根据需要添加更严格的比较逻辑

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
            // ✅ 使用自定义函数判断边是否相同
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

void MainWindow::extractFace()
{
    // 你的实现代码
}
