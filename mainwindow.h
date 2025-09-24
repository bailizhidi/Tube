#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>

// 在包含 OpenCASCADE 头文件之前，抑制弃用警告
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
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

#include <queue>
#include <set>
#include <map>
#include <vtkCellPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <TopTools_ListOfShape.hxx>
// 在包含完 OpenCASCADE 头文件之后，恢复警告设置
#pragma GCC diagnostic pop

#include <vtkCylinderSource.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>

#include <functional>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void make_elbow_model(); // 弯管建模
    void import_part(); // 导入数模
    void extractFace(); // 提取外表面

private:
    Ui::MainWindow *ui;
    TopoDS_Shape m_currentShape; // 保存当前加载的模型

    void MakeElbowModel(
        double R_out, double R_in, double length,        // 管体外半径、内半径、长度
        double sleeve_thickness1, double sleeve_length1,   // 旋转套
        double sleeve_thickness, double sleeve_length,   // 固定套
        double rotary_pos, double fixed_pos,             // 套筒位置（距管体右端的距离）
        double arc_R, double arc_t, double arc_angle // 半圆弧套筒半径、厚度、角度（单位：弧度）
        );
    // 辅助函数声明
    vtkSmartPointer<vtkPolyData> ConvertOCCShapeToVTKPolyData(const TopoDS_Shape& shape, double linearDeflection);
    vtkSmartPointer<vtkActor> CreateVTKActor(vtkSmartPointer<vtkPolyData> polyData,
                                             double r, double g, double b);
    // 在MDI子窗口中显示模型的函数
    void ShowModelInMdiArea(vtkSmartPointer<vtkRenderer> renderer);

    // 读取STEP文件
    TopoDS_Shape ReadSTEPFile(const QString& fileName);
    // 读取IGES文件
    TopoDS_Shape ReadIGESFile(const QString& fileName);
    // 将OCC形状显示到ui->mdiArea（复用渲染逻辑）
    void DisplayShape(const TopoDS_Shape& shape);
    // 用OCC提取表面
    // 存储 OCC Face 和 VTK CellId 的映射
    std::map<vtkIdType, TopoDS_Face> m_faceMap;
    // 鼠标点击回调
    //static void OnLeftButtonDown(vtkObject* obj, unsigned long eid, void* clientdata, void* calldata);
    // 核心函数：扩展查找外表面
    //TopoDS_Shape FindConnectedOuterSurface(const TopoDS_Shape& shape, const TopoDS_Face& seedFace);
    // 辅助函数
    void GetFacesSharingEdge(const TopoDS_Shape& shape, const TopoDS_Edge& edge, TopTools_ListOfShape& faceList);
    bool AreFacesOnSameSide(const TopoDS_Face& f1, const TopoDS_Face& f2);
    gp_Vec GetFaceNormal(const TopoDS_Face& face);
    static void OnLeftButtonDown(vtkObject* caller, unsigned long eventId, void* clientData, void* callData);

    // 自定义哈希函数
    struct ShapeHash {
        std::size_t operator()(const TopoDS_Shape& shape) const {
            return std::hash<TopoDS_Shape*>{}(const_cast<TopoDS_Shape*>(&shape));
        }
    };

    // 自定义相等比较函数
    struct ShapeEqual {
        bool operator()(const TopoDS_Shape& lhs, const TopoDS_Shape& rhs) const {
            return lhs.IsSame(rhs);
        }
    };

    TopoDS_Shape FindConnectedOuterSurface(const TopoDS_Shape& shape, const TopoDS_Face& seedFace);
    void GetFaceCenterAndNormal(const TopoDS_Face& face, gp_Pnt& center, gp_Vec& normal);
    bool IsFaceInFront(const TopoDS_Face& face, const gp_Pnt& seedCenter, const gp_Vec& seedNormal);
};
#endif // MAINWINDOW_H
