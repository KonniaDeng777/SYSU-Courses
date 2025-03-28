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

% 信道（有线，没有噪声）
receivedSignal = modData;

% 解调
demodData = pskdemod(receivedSignal, M);

% 解码
receivedData = demodData;

% 分析接收效果
numErrors = sum(data ~= receivedData);
disp(['Number of errors without noise(wired): ', num2str(numErrors)]);
