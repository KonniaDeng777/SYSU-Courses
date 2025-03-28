% 初始化
clc;
clear;

% 生成随机二进制数据
dataSize = 1000; % 数据大小
data = randi([0 1], dataSize, 1);

% 编码（单极性编码）
encodedData = data;

% 调制方式（例如2ASK）
M = 2; % 2ASK
modData = pskmod(encodedData, M);

% 设置无线信道模型
Fs = 1000e3; % 采样率
fc = 2.4e9; % 载波频率 2.4 GHz
channel = comm.AWGNChannel('NoiseMethod', 'Signal to noise ratio (SNR)', 'SNR', 10);

% 添加AWGN噪声
noisySignal = channel(modData);

% 使用无线信道模型进行传输
% 这里简化为单纯的AWGN信道模型
receivedSignal = noisySignal;

% 解调
demodData = pskdemod(receivedSignal, M);

% 解码
receivedData = demodData;

% 分析接收效果
numErrors = sum(data ~= receivedData);
disp(['Number of errors without noise(wireless): ', num2str(numErrors)]);
