# Teamwork-Minecraft

The project is intended to implement a simple Minecraft, for learning purposes only.



# 开发日志

## Part1(hjl)

### 内容：

基本代码架构、包括，游戏窗口、游戏状态设置（关闭、鼠标设置）、相机设置(第一人称)，基本渲染引擎（纹理加载）。



### 遇到问题：

#### 一、易读性问题：纹理、相机、需不需要进一步抽象？



#### 二、第一人称的鼠标居中问题：

**解决：**鼠标上一次位置应时刻置为屏幕中心，



#### 三、纹理加载问题：LINEAR与NEAREST的使用、颜色混合问题、模糊问题

**解决：**

在加载纹理的滤波函数用GL_NEAREST解决近距离模糊，使用mipmap解决远距离过于锐化的问题。

帧更新调用repaint()立即渲染,而不是update()进入队列，加速渲染。



## Part2 (Team)

### 一、人物模型与视角绑定(hjl)

#### **11/25**

#### **纹理加速：**

同一绘图部分的纹理集成到一张贴图，增加缓存命中以加速。**带来的问题是：手动计算复杂；边缘的纹理坐标很难精确，容易越界到另一个纹理区域。**

![image-20231127213512289](C:\Users\Aaron Inkin\AppData\Roaming\Typora\typora-user-images\image-20231127213512289.png)

#### **建立了基本人物模型：**

头(8*8\*8),身(8\*4\*12),手臂及腿(4\*4\*12)，以及纹理，纹理使用mc原贴图(在一张图上)。

![image-20231127213438656](C:\Users\Aaron Inkin\AppData\Roaming\Typora\typora-user-images\image-20231127213438656.png)

#### **运动摆动：**

在键盘事件中，加入旋转角度更新，并在每一帧更新旋转手臂和腿。

![image-20231127213606666](C:\Users\Aaron Inkin\AppData\Roaming\Typora\typora-user-images\image-20231127213606666.png)

#### 额外地：

规定方块大小16\*16，坐标系中为0.16\*0.16



#### 11/27完成该部分

#### 人物头部随相机转动而转动

实现思路：目标是将人物方块的前面移至跟相机方向垂直。由于我们相机有一个`cameraFront`变量表示看向的方向向量，人物初始朝向为(0, 0, 1)，z轴，因此计算`cameraFront`和初始朝向的叉乘，得到二者平面的法向量，那么我们可以利用这个法向量作为旋转轴，旋转角度为从(0,0,1)旋转至`cameraFront`的位置。

![image-20231127221434366](C:\Users\Aaron Inkin\AppData\Roaming\Typora\typora-user-images\image-20231127221434366.png)

#### 人物身体随相机转动而转动

实现思路：跟上面头部不同，因为根据现实状况，人物身体只能绕y轴改变方向，因此应该先将`cameraFront`投影到xoz平面上，再做进一步计算。

![image-20231127221759559](C:\Users\Aaron Inkin\AppData\Roaming\Typora\typora-user-images\image-20231127221759559.png)

#### 相机绑定

这个绑定比较简单，设置初始化的相机位置在头部即可，

![image-20231127223104291](C:\Users\Aaron Inkin\AppData\Roaming\Typora\typora-user-images\image-20231127223104291.png)

#### 切换人称

还实现了一个简单的按键切换人称：按下c后将相机放置在人物前上方。

![image-20231127224236507](C:\Users\Aaron Inkin\AppData\Roaming\Typora\typora-user-images\image-20231127224236507.png)

![image-20231127224322360](C:\Users\Aaron Inkin\AppData\Roaming\Typora\typora-user-images\image-20231127224322360.png)



#### 简单实现了一个手臂摆动：

在运动时会持续旋转手臂。

![image-20231127224513279](C:\Users\Aaron Inkin\AppData\Roaming\Typora\typora-user-images\image-20231127224513279.png)



#### 最终效果：





### 二、碰撞检测（fjy）



### 三、基本方块纹理和渲染 （hmk）



## Part3

### 天空盒(球体？)

### 世界生成(随机种子，树，高山，地下)

### 物品交互系统(简单的破坏，放置)



##  Part4

### 环境优化:

### 天气

### 局部光照

### 火焰

### 水流































