#  2024Spring计算机图形学

纯原生C++ / OpenGL图形学项目.
> Xcode C++ compiler:
> Apple clang version 15.0.0 (clang-1500.3.9.4)
> Target: arm64-apple-darwin23.5.0
> Thread model: posix

## Dependencies
| 依赖名称 | 当前版本 | 最低版本 | 说明 |
| -- | -- | -- | -- |
| glad | 4.1 | 3.3 | OpenGL核心库 |
| GLEW | 2.2.0 | 2.1.0 | 加载 OpenGL 函数指针 |
| GLFW | 3.4.0 | 3.3.0 | 创建窗口、管理上下文、处理输入等 OpenGL 相关的基础功能 |
| glm | 1.0.1 | 1.0.0 | 提供OpenGL各种数学实体和功能|

还可能需要GDAL或SFLM用于文件读写.

使用ImGUI进行GUI绘制.

## 已实现的功能(基本图形引擎)

- 图元绘制类行为的封装
- 渲染器类行为的封装
- 摄影机类行为的封装
- 各类基本键鼠输入记录参数,绘制状态参数,窗口状态参数管理
- 使用键盘滚轮进行视窗缩放和移动
- 使用鼠标拖动绘制基本图元
- 使用鼠标点选绘制基本图元
- 使用imgui构建GUI框架
- 从文件读入渲染器行为
- 通过imgui的color panel生成不同颜色的渲染器
- 对每一个图元绑定唯一的渲染器实例

## 开发内容1: 矢量元素绘制

- [ ]  完成完整各类图元的绘制
- [ ]  实现图元的平移旋转缩放
- [ ]  完成更多绘制样式定义: 点大小, 线宽, 线样式, 不透明度, 多边形填充/ 不填充
- [ ]  绘制一些基本的Marker

## 开发内容2: 点线面表拓扑建立

- [ ]  将绘制图元类进一步整合为拓扑图元类
- [ ]  获取当前鼠标位置的拓扑图元类
- [ ]  实现拓扑图元类选中逻辑
- [ ]  生成预览样式和外界矩形用于修改

## 开发内容3: 文件交互与栅格加载

- [ ]  进一步完善渲染器文件接口
- [ ]  基于第三方库封装栅格影像读写接口
- [ ]  实现对栅格影像的平移旋转缩放修改不透明度操作
- [ ]  实现矢量元素的读写接口

## 开发内容4: GUI完善和图层逻辑绑定

- [ ]  完成窗口堆叠显示逻辑和各类功能接口
- [ ]  生成当前图像中的图元对象的列表
- [ ]  绑定列表操作与拓扑图元选中,修改和显式顺序

## 开发内容5: 绑定地理坐标系

- [ ]  实现当前矢量信息标准坐标的导出
- [ ]  构建地理坐标系样式文件
- [ ]  绑定地理坐标系与当前投影坐标系