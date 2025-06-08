import numpy as np
import matplotlib.pyplot as plt

# ============================= 参数配置 =============================
freq = 50            # 正弦波频率 (Hz)
modulation_index = 0.8  # 调制指数 (0~1.15)
sampling_points = 1000  # 采样点数
Vdc = 1.0            # 直流母线电压 (归一化值)
# ==================================================================

# 生成时间轴
t = np.linspace(0, 1/freq, sampling_points)
theta = 2 * np.pi * freq * t  # 角度随时间线性增加

# 生成三相正弦波 (相位差120°)
U = modulation_index * np.sin(theta)
V = modulation_index * np.sin(theta - 2*np.pi/3)
W = modulation_index * np.sin(theta + 2*np.pi/3)

# ======================== Clarke变换 (ABC -> αβ) ========================
def clarke_transform(u, v, w):
    alpha = (2/3) * (u - 0.5*v - 0.5*w)
    beta = (2/3) * (np.sqrt(3)/2*v - np.sqrt(3)/2*w)
    return alpha, beta

alpha, beta = clarke_transform(U, V, W)

# ======================== SVPWM扇区判断 ========================
def get_sector(alpha, beta):
    angle = np.arctan2(beta, alpha)  # 计算矢量角度
    sector = np.floor((angle + np.pi/6) / (np.pi/3)) % 6
    return sector.astype(int)

sector = get_sector(alpha, beta)

# ======================== 计算矢量作用时间 ========================
def calculate_times(alpha, beta, sector):
    T = np.zeros((len(alpha), 3))  # T0, T1, T2
    for i in range(len(alpha)):
        a = alpha[i]
        b = beta[i]
        s = sector[i]
        
        # 根据扇区选择计算系数
        if s == 0:
            X = (np.sqrt(3) * a - b) * (1/np.sqrt(3))
            Y = (2 * b) * (1/np.sqrt(3))
        elif s == 1:
            X = (np.sqrt(3) * a + b) * (1/np.sqrt(3))
            Y = (-np.sqrt(3) * a + b) * (1/np.sqrt(3))
        # 其他扇区类似...
        # 简化为通用公式:
        T1 = (a * np.sin((s+1)*np.pi/3) - b * np.cos((s+1)*np.pi/3)) * (2/np.sqrt(3))
        T2 = (-a * np.sin(s*np.pi/3) + b * np.cos(s*np.pi/3)) * (2/np.sqrt(3))
        
        # 归一化时间
        T_sum = T1 + T2
        T0 = 1 - T_sum
        T[i] = [T0, T1, T2]
    return T

T = calculate_times(alpha, beta, sector)

# ======================== 生成PWM占空比 ========================
def generate_pwm_duty(T, sector):
    duty = np.zeros((len(T), 3))  # 三相占空比
    for i in range(len(T)):
        s = sector[i]
        T0, T1, T2 = T[i]
        
        # 七段式SVPWM分配
        t0 = T0/2
        t1 = T1/2
        t2 = T2/2
        
        if s == 0:
            duty[i] = [t1 + t2 + t0, t2 + t0, t0]
        elif s == 1:
            duty[i] = [t1 + t0, t1 + t2 + t0, t0]
        # 其他扇区类似...
    return duty

duty = generate_pwm_duty(T, sector)

# ======================== 可视化结果 ========================
plt.figure(figsize=(15, 10))

# 1. 原始三相正弦波
plt.subplot(3, 1, 1)
plt.plot(t, U, label='U Phase')
plt.plot(t, V, label='V Phase')
plt.plot(t, W, label='W Phase')
plt.title("Original Three-Phase Sine Waves")
plt.xlabel("Time (s)")
plt.ylabel("Voltage")
plt.legend()
plt.grid(True)

# 2. α-β分量与电压矢量
plt.subplot(3, 1, 2)
plt.plot(alpha, beta, 'b-', label='Voltage Vector Trajectory')
plt.title("α-β Plane Voltage Vector")
plt.xlabel("α")
plt.ylabel("β")
plt.axis('equal')
plt.grid(True)

# 3. 生成的PWM占空比
plt.subplot(3, 1, 3)
plt.plot(t, duty[:,0], 'r-', label='U Duty')
plt.plot(t, duty[:,1], 'g-', label='V Duty')
plt.plot(t, duty[:,2], 'b-', label='W Duty')
plt.title("Generated PWM Duty Cycles")
plt.xlabel("Time (s)")
plt.ylabel("Duty Ratio")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()
