### 瑞芯微RV1106智能监控终端

### 视频监控将由ipc-terminal、ipc-client组成

### 本工程是网络摄像头摄像头端的板端软件ipc-terminal

### 连线，luckfox-pico-max 开发板

![Alt text](luckfox-pico-max.png)

实物图：

![Alt text](实物图.jpg)

摄像头使用的 sc3336，固定于舵机云台上。

如果使用官方支持的屏幕，在开发板命令行输入 luckfox-config，再使能对应外设即可。

我的屏幕使用的st7735s 1.8寸屏幕，官方SDK中有驱动，但需要自己配置 tbtft 设备节点，参考大佬教程：

https://blog.csdn.net/qq_46205054/article/details/142266317

使用 pwm 需要进入板子系统，使能 pwm8 pwm9 字符设备驱动
```bash
luckfox-config
```

![Alt text](luckfox-config-pwm89.png)

### 使用步骤
1. 安装依赖环境
```bash
sudo apt update
sudo apt-get install -y git ssh make gcc gcc-multilib g++-multilib module-assistant expect g++ gawk texinfo libssl-dev bison flex fakeroot cmake unzip gperf autoconf device-tree-compiler libncurses5-dev pkg-config bc python-is-python3 passwd openssl openssh-server openssh-client vim file cpio rsync
```

2. 拉取luckfox-pico官方SDK
```bash
git clone https://gitee.com/LuckfoxTECH/luckfox-pico.git
```

3. 拉取ipc-terminal工程
```bash
git clone https://github.com/xiaoLann2001/ipc-terminal.git
```

4. 编译
```bash
mkdir build
cd build
export SDK_PATH=/path/to/your/luckfox-pico
cmake ..
make install
```

5. 将install目录下所有文件拷贝到开发板即可，ipc-terminal.ini 中可以修改软件参数，使能/关闭软件功能，每次运行前修改

6. ipc-server测试，服务器地址和端口在 net.h 中修改

### 各个模块功能支持列表：

- **网络模块`Network`**
    - **连接服务器**：通过套接字（socket）连接到指定的IP地址和端口，支持自动重连。
    - **数据发送**：提供`send_data`方法，将数据放入队列，并通过单独的线程发送到服务器。
    - **数据接收**：使用`poll`机制在独立线程中接收服务器返回的数据，并通过`signal_network_received`信号进行事件通知。
    - **NTP同步**：通过单独的NTP线程定时向指定的NTP服务器发送请求以同步时间。
    - **线程管理**：包括接收、发送、NTP等线程的创建和管理。
    - **资源释放**：在析构函数中关闭网络连接并释放相关资源。
    - **线程同步**：通过多个条件变量和互斥锁（`std::mutex`）确保线程间的同步。
    - **网络状态监控**：实时监控网络连接状态，若连接断开，则自动尝试重新连接。
    - **多线程支持**：支持多线程操作，分离接收线程、发送线程和NTP同步线程，避免主线程阻塞。

- **控制模块`Control`**
    - **控制函数注册**：通过`registerControlFunction`方法注册控制函数，将模块ID和操作码与相应的函数进行绑定。
    - **信号解析**：通过`parseSignal`方法解析从网络接收到的控制信号，支持解析模块ID、操作码、参数。
    - **信号分发**：通过`dispatchSignal`方法，将解析后的控制信号分发到相应的模块执行相应的控制函数。
    - **网络数据接收**：通过`onNetworkReceived`方法接收网络数据，解析并分发控制信号。
    - **模块支持**：支持多个模块的控制信号解析，包括LED、云台（Pantilt）、显示等。
    - **错误处理**：对未知的模块ID、操作码或参数进行错误处理，并打印日志。

- **视频处理模块`Video`**
    - **视频流初始化**：通过调用RKAIQ、RKMPI、VI设备等初始化接口，初始化视频处理相关设备和模块。
    - **多通道视频管道支持**：支持多个视频管道同时处理，分别通过`video_pipe0`、`video_pipe1`、`video_pipe2`等方法处理不同的视频流。
    - **视频编码与RTSP推流**：将视频帧通过VENC模块进行编码，并通过RTSP协议发送到RTSP服务器。
    - **AI目标检测**：通过AI推理模型（如YOLOv5），检测视频中的目标（如人物、车辆、宠物等），并支持动态框架（OSD）叠加。
    - **AI目标跟踪**：支持选择并跟踪视频中的指定目标，并且根据目标位置进行动态调整。
    - **视频帧转换与显示**：将视频帧转换为不同的格式（如YUV到BGR），并在需要时进行显示。
    - **帧率显示**：通过OpenCV的`cv::putText`方法在视频帧上显示实时的帧率。
    - **多线程支持**：每个视频管道在独立的线程中运行，避免阻塞其他操作，提升系统并发性能。

    - **视频管道功能**
        - **管道0 (`video_pipe0`)**
            - 捕获视频帧并进行编码后通过RTSP发送。
            - 管道运行与停止受`video_run_`和`pipe0_run_`标志控制。
        - **管道1 (`video_pipe1`)**
            - 处理另一个视频流，支持不同的分辨率。
            - 支持OpenCV的`cv::cvtColor`和`cv::resize`方法对帧进行处理和转换。
            - 支持显示帧率，实时更新并叠加在视频帧上。
        - **管道2 (`video_pipe2`)**
            - 专门处理AI目标检测和跟踪，识别视频中的目标，并支持目标框的绘制。
            - 支持动态配置跟踪目标（如人物、车辆等）。

    - **资源管理**
        - **内存管理**：通过`malloc`分配内存，使用完毕后通过`free`释放，确保没有内存泄漏。
        - **视频帧释放**：使用`venc_release_frame`和`vi_release_frame`方法，确保每一帧在处理完成后能够被释放。

    - **AI目标检测与跟踪功能**
        - **目标检测**：通过YOLOv5等模型进行目标检测，支持配置哪些目标类别需要被检测。
        - **目标框绘制**：检测到目标后，通过AI OSD功能将目标框绘制到视频帧上。
        - **目标跟踪**：支持通过AI跟踪特定目标，并可以动态调整跟踪区域。

    - **线程同步与管理**
        - **线程管理**：通过`std::thread`创建多个视频处理线程，确保每个视频管道的并行处理。
        - **线程同步**：通过`std::mutex`等同步机制，确保多个线程在共享资源时不会发生冲突。

    - **参数化与动态配置**
        - **配置读取**：通过`rk_param_get_int`函数读取配置文件中的视频和AI参数，支持动态调整视频流宽高、AI跟踪设置等。
        - **灵活配置**：支持用户根据需求调整视频处理和AI检测的行为，例如是否启用AI检测，是否跟踪特定类别的目标。

    - **资源释放**
        - **析构函数资源释放**：在`~Video()`析构函数中，确保各个模块（如RTSP、VENC、VI等）在销毁时被正确释放，避免资源泄露。
        - **线程退出管理**：确保视频处理线程能够在模块销毁时正常退出，避免程序异常终止。

- **Led模块`Led`**
    - **初始化LED**：通过构造函数`Led::Led`初始化LED，设置LED编号，并通过GPIO进行初始化。
    - **关闭LED**：通过`Led::off`方法关闭LED。
    - **开启LED**：通过`Led::on`方法打开LED。
    - **切换LED状态**：通过`Led::toggle`方法切换LED的状态（开/关）。
    - **闪烁LED**：通过`Led::blink`方法控制LED的闪烁，支持设置闪烁持续时间（秒）和频率。
    - **设置工作模式**：通过`Led::set_mode`方法设置LED的工作模式（如手动模式或定时模式）。
    - **设置点亮时间**：通过`Led::set_on_time`方法设置LED的点亮时间（毫秒）。
    - **设置熄灭时间**：通过`Led::set_off_time`方法设置LED的熄灭时间（毫秒）。
    - **设置闪烁频率**：通过`Led::set_blink_frequency`方法设置LED的闪烁频率，支持1到3的频率范围。
    - **GPIO初始化与释放**：在构造函数中初始化GPIO，析构函数中释放GPIO资源。

- **Pantilt模块`Pantilt`**
    - **初始化舵机**：通过构造函数`Pantilt::Pantilt`初始化PWM接口，用于控制俯仰（180°舵机）和旋转（360°舵机），设置PWM周期、占空比及极性。
    - **复位舵机**：通过`Pantilt::reset`方法将舵机复位到预设角度。
    - **设置俯仰角度**：通过`Pantilt::setTilt`方法调整俯仰角度，限制在指定范围内（-60°到90°），并根据角度计算PWM占空比进行控制。
    - **设置旋转角度**：通过`Pantilt::setPan`方法调整旋转角度，限制在指定范围内（-90°到90°），并根据角度计算PWM占空比进行控制。
    - **调整云台角度**：通过`Pantilt::onAjustPantilt`方法同时调整旋转和俯仰角度，步长限制为`pan_step_limit`和`tilt_step_limit`。
    - **上下俯仰角度调整**：通过`Pantilt::up`和`Pantilt::down`方法调整俯仰角度。
    - **左右旋转角度调整**：通过`Pantilt::left`和`Pantilt::right`方法调整旋转角度。
    - **预设角度**：通过读取配置文件获取预设角度和回位时间。
    - **归位检查线程**：使用`Pantilt::homePositionCheck`方法检查舵机是否已归位，并在必要时执行归位操作。
    - **占空比映射**：通过`Pantilt::mapAngleToDutyCycle`方法将角度映射为PWM占空比。

- **Display模块`Display`**
    - **初始化显示设备**：通过构造函数`Display::Display`初始化Framebuffer设备，自动获取分辨率和色深。
    - **显示线程**：启动一个显示线程`Display::display_on_fb`，该线程负责从队列中取帧并显示到Framebuffer设备。
    - **推送帧到显示队列**：通过`Display::push_frame`方法将新的帧推送到显示队列，且如果队列未满，显示线程会被唤醒处理该帧。
    - **暂停显示**：通过`Display::pause`方法暂停显示，进入暂停状态时，显示线程会等待唤醒信号。
    - **继续显示**：通过`Display::resume`方法恢复显示，清空队列并通知显示线程继续工作。
    - **退出显示线程**：通过析构函数`Display::~Display`设置退出标志，并唤醒所有等待的线程，等待显示线程结束后释放资源。
    - **队列限制**：在`Display::push_frame`方法中限制队列长度，避免内存占用过多。
    - **帧类型和尺寸校验**：在`Display::display_on_fb`方法中，校验帧类型和尺寸是否与显示设备兼容，若不匹配则进行调整。
    - **颜色格式转换**：通过`cv::cvtColor`将帧的颜色格式从RGB转换为BGR565格式，以适应Framebuffer的显示要求。
    - **显示到Framebuffer**：通过`framebuffer_set_frame_rgb565`将处理后的帧显示到Framebuffer设备。

- **Signal模块`Signal`**
    - **支持选择信号与槽的实现方式**：直接回调和事件循环回调
    - **直接回调**：
        - **连接成员函数**：通过`Signal::connect`方法将成员函数连接到信号，支持通过成员函数调用对象的相关操作。
        - **连接普通函数**：通过`Signal::connect`方法将普通函数连接到信号，能够绑定一个普通函数作为槽。
        - **线程安全操作**：通过`std::mutex`保证连接、断开、发射信号等操作在多线程环境下的安全性。
        - **断开连接**：通过`Signal::disconnect`方法可以断开已连接的槽，支持删除特定槽的功能。
        - **发射信号**：通过`Signal::emit`方法发射信号，触发所有连接的槽函数进行执行。
        - **支持多重参数**：支持传递多个参数给槽函数，可以连接任何支持接收这些参数的函数。
        - **智能指针管理**：使用`std::shared_ptr`存储槽函数，确保在槽函数被销毁时能够正确释放资源，避免内存泄漏。
        - **成员函数参数重载**：支持连接不同参数形式的成员函数，包括常量引用参数，增强了函数连接的灵活性。
        - **优点**：
            - 实现简单，适合直接回调的场景。
            - 不需要额外的线程开销，性能较高。
            - 响应速度快，适合不需要复杂调度的实时操作。
        - **缺点**：
            - 在多线程环境下，可能会遇到信号发射时阻塞的问题，尤其是执行耗时操作时。
            - 如果信号发射频繁，可能会导致性能瓶颈。
            - 不支持异步执行，所有槽函数是同步执行的。
    - **事件循环回调**：
        - **自动启动事件循环**：构造`Signal`对象时自动启动事件循环线程，并且在析构时停止事件循环，避免手动管理。
        - **事件队列**：信号的发射会将槽函数任务加入队列，由事件循环线程依次执行。
        - **线程安全**：事件队列、信号发射以及槽函数调用的操作都是线程安全的，确保在多线程环境下的正确执行。
        - **智能指针管理**：同样使用`std::shared_ptr`来管理槽，确保槽函数对象在事件循环中被正确销毁。
        - **异步事件处理**：通过事件循环的机制实现异步事件处理，适用于处理需要延迟执行的操作。
        - **优点**：
            - 适合高频率的事件发射和需要异步处理的场景，能够将槽函数的执行延迟。
            - 避免了直接回调中的阻塞问题，因为槽函数是在独立线程中执行的。
            - 支持异步处理，可以在不阻塞主线程的情况下处理事件。
        - **缺点**：
            - 事件循环机制的实现相对复杂，增加了线程管理的开销。
            - 可能会引入额外的延迟，因为事件是在事件循环中按队列顺序执行的。
            - 如果事件过多，可能会导致事件队列堆积，影响性能。

- **参数模块`param`**
    - **动态调整与配置**：支持从配置文件（如`rk_param_get_int`）读取视频流和AI参数，动态调整视频流宽高、AI检测启用状态等。
    - **参考rkipc `param.c`**

- **日志模块`log`**
    - **支持打印调用函数，所在文件，以及日志级别管理**
    - **参考rkipc `log.c`**

- **引入开放网络视频协议`Onvif`**
    - **支持设备发现**
    - **支持云台控制**




