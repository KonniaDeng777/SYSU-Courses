% 初始化
clc;
clear;

% 参数设置
dataSize = 1000; % 数据大小
SNR = 10; % 信噪比
Fs = 10e3; % 采样率，调整为更合理的值
freqSep = 1000; % 频率分离，确保小于Fs/2
nsamp = 4; % 每符号样本数

% 生成随机二进制数据
data = randi([0 1], dataSize, 1);

% 编码方式：单极性、双极性、差分编码
encodedData_unipolar = data; % 单极性编码
encodedData_bipolar = 2*data - 1; % 双极性编码
encodedData_diff = xor(data, [0; data(1:end-1)]); % 差分编码


% 调制方式：2ASK, 2FSK, 2PSK
% 2ASK 调制
modData_2ask = 2*encodedData_unipolar - 1;

% 2FSK 调制
M = 2;
encodedData_bipolar(encodedData_bipolar == -1) = 0;
encodedData_diff = double(encodedData_diff);
modData_2fsk = fskmod(encodedData_unipolar, M, freqSep, nsamp, Fs);

% 2PSK 调制
modData_2psk = pskmod(encodedData_unipolar, M);

% 模拟有线信道（添加AWGN噪声）
noisySignal_2ask = awgn(modData_2ask, SNR, 'measured');
noisySignal_2fsk = awgn(modData_2fsk, SNR, 'measured');
noisySignal_2psk = awgn(modData_2psk, SNR, 'measured');

% 解调
demodData_2ask = noisySignal_2ask > 0;
demodData_2fsk = fskdemod(noisySignal_2fsk, M, freqSep, nsamp, Fs);
demodData_2psk = pskdemod(noisySignal_2psk, M);

% 解码
receivedData_2ask = demodData_2ask;
receivedData_2fsk = demodData_2fsk;
receivedData_2psk = demodData_2psk;

% 分析接收效果
numErrors_2ask = sum(data ~= receivedData_2ask);
numErrors_2fsk = sum(data ~= receivedData_2fsk);
numErrors_2psk = sum(data ~= receivedData_2psk);

disp(['Number of errors with 2ASK modulation: ', num2str(numErrors_2ask)]);
disp(['Number of errors with 2FSK modulation: ', num2str(numErrors_2fsk)]);
disp(['Number of errors with 2PSK modulation: ', num2str(numErrors_2psk)]);

% 使用无线信道
rayleighChannel = comm.RayleighChannel('SampleRate', Fs, 'PathDelays', [0 1e-6 2e-6], ...
    'AveragePathGains', [0 -3 -6], 'NormalizePathGains', true);

% 调用 release 方法释放之前的配置
release(rayleighChannel);
rayleighSignal_2ask = rayleighChannel(modData_2ask);
release(rayleighChannel);
rayleighSignal_2fsk = rayleighChannel(complex(modData_2fsk));
release(rayleighChannel);
rayleighSignal_2psk = rayleighChannel(modData_2psk);

% 添加AWGN噪声
noisyRayleighSignal_2ask = awgn(rayleighSignal_2ask, SNR, 'measured');
noisyRayleighSignal_2fsk = awgn(rayleighSignal_2fsk, SNR, 'measured');
noisyRayleighSignal_2psk = awgn(rayleighSignal_2psk, SNR, 'measured');

% 解调
demodRayleighData_2ask = noisyRayleighSignal_2ask > 0;
demodRayleighData_2fsk = fskdemod(noisyRayleighSignal_2fsk, M, freqSep, nsamp, Fs);
demodRayleighData_2psk = pskdemod(noisyRayleighSignal_2psk, M);

% 解码
receivedRayleighData_2ask = demodRayleighData_2ask;
receivedRayleighData_2fsk = demodRayleighData_2fsk;
receivedRayleighData_2psk = demodRayleighData_2psk;

% 分析接收效果
numErrorsRayleigh_2ask = sum(data ~= receivedRayleighData_2ask);
numErrorsRayleigh_2fsk = sum(data ~= receivedRayleighData_2fsk);
numErrorsRayleigh_2psk = sum(data ~= receivedRayleighData_2psk);

disp(['Number of errors with 2ASK modulation in Rayleigh channel: ', num2str(numErrorsRayleigh_2ask)]);
disp(['Number of errors with 2FSK modulation in Rayleigh channel: ', num2str(numErrorsRayleigh_2fsk)]);
disp(['Number of errors with 2PSK modulation in Rayleigh channel: ', num2str(numErrorsRayleigh_2psk)]);

%% 绘图
figure;
subplot(3, 1, 1);
plot(modData_2ask);
title('2ASK 调制信号（无噪声）');
xlabel('时间');
ylabel('幅度');

subplot(3, 1, 2);
plot(noisySignal_2ask);
title('2ASK 调制信号（有AWGN噪声）');
xlabel('时间');
ylabel('幅度');

subplot(3, 1, 3);
plot(noisyRayleighSignal_2ask);
title('2ASK 调制信号（Rayleigh信道+AWGN噪声）');
xlabel('时间');
ylabel('幅度');

figure;
subplot(3, 1, 1);
plot(modData_2fsk);
title('2FSK 调制信号（无噪声）');
xlabel('时间');
ylabel('幅度');

subplot(3, 1, 2);
plot(noisySignal_2fsk);
title('2FSK 调制信号（有AWGN噪声）');
xlabel('时间');
ylabel('幅度');

subplot(3, 1, 3);
plot(noisyRayleighSignal_2fsk);
title('2FSK 调制信号（Rayleigh信道+AWGN噪声）');
xlabel('时间');
ylabel('幅度');

figure;
subplot(3, 1, 1);
plot(modData_2psk);
title('2PSK 调制信号（无噪声）');
xlabel('时间');
ylabel('幅度');

subplot(3, 1, 2);
plot(noisySignal_2psk);
title('2PSK 调制信号（有AWGN噪声）');
xlabel('时间');
ylabel('幅度');

subplot(3, 1, 3);
plot(noisyRayleighSignal_2psk);
title('2PSK 调制信号（Rayleigh信道+AWGN噪声）');
xlabel('时间');
ylabel('幅度');

% 保存图像
output_folder = 'output_images';
figure(1);
saveas(gcf, fullfile(output_folder, 'unipolar_modulated_signals_2ASK.png'));

figure(2);
saveas(gcf, fullfile(output_folder, 'unipolar_modulated_signals_2FSK.png'));

figure(3);
saveas(gcf, fullfile(output_folder, 'unipolar_modulated_signals_2PSK.png'));
