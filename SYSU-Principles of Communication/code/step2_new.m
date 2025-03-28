clc;
clear;
close all;

%% 参数设置
data_length = 100; % 数据长度
fs = 100; % 采样频率
t = 0:1/fs:(data_length-1/fs); % 时间轴

%% 生成随机二进制数据
data = randi([0, 1], 1, data_length);
disp('原始数据:');
disp(data);

%% 编码方式
% 单极性NRZ编码
encoded_unipolar = data;

% 双极性NRZ编码
encoded_bipolar = 2 * data - 1;

% 差分编码
encoded_diff = zeros(1, data_length);
encoded_diff(1) = data(1);
for i = 2:data_length
    encoded_diff(i) = xor(data(i), data(i-1));
end

%% 调制方式
% 扩展数据长度以适应采样频率
extended_data = repmat(encoded_diff, fs, 1);
extended_data = extended_data(:)';

% 2ASK 调制
A1 = 1; A2 = 2; % 0 和 1 的幅度
modulated_2ASK = A1 + (A2 - A1) * extended_data;

% 2FSK 调制
f1 = 1; f2 = 2; % 0 和 1 的频率
t_per_symbol = 0:1/fs:1-1/fs; % 每个符号的时间
modulated_2FSK = [];
for i = 1:data_length
    if encoded_unipolar(i) == 0
        modulated_2FSK = [modulated_2FSK sin(2*pi*f1*t_per_symbol)];
    else
        modulated_2FSK = [modulated_2FSK sin(2*pi*f2*t_per_symbol)];
    end
end

% 2PSK 调制
modulated_2PSK = cos(2*pi*f1*t + pi*extended_data);

%% 解调
% 2ASK 解调
threshold_2ASK = (A1 + A2) / 2;
received_2ASK = modulated_2ASK > threshold_2ASK;
received_2ASK = received_2ASK(1:fs:end);

% 2FSK 解调
received_2FSK = zeros(1, data_length);
for i = 1:data_length
    segment = modulated_2FSK((i-1)*fs + 1:i*fs);
    if mean(segment .* sin(2*pi*f1*t_per_symbol)) > mean(segment .* sin(2*pi*f2*t_per_symbol))
        received_2FSK(i) = 0;
    else
        received_2FSK(i) = 1;
    end
end

% 2PSK 解调
received_2PSK = zeros(1, data_length);
for i = 1:data_length
    segment = modulated_2PSK((i-1)*fs + 1:i*fs);
    if mean(segment .* cos(2*pi*f1*t_per_symbol)) > 0
        received_2PSK(i) = 0;
    else
        received_2PSK(i) = 1;
    end
end

%% 分析接收效果
% 计算误码率
ber_2ASK = sum(data ~= received_2ASK) / data_length;
ber_2FSK = sum(data ~= received_2FSK) / data_length;
ber_2PSK = sum(data ~= received_2PSK) / data_length;

disp(['2ASK 误码率: ', num2str(ber_2ASK)]);
disp(['2FSK 误码率: ', num2str(ber_2FSK)]);
disp(['2PSK 误码率: ', num2str(ber_2PSK)]);

%% 绘图
figure(1);
subplot(3, 1, 1);
stairs([0:data_length], [encoded_unipolar encoded_unipolar(end)], 'linewidth', 2);
title('单极性NRZ编码信号');
xlabel('时间');
ylabel('幅度');
axis([0 data_length -0.5 1.5]);

subplot(3, 1, 2);
stairs([0:data_length], [encoded_bipolar encoded_bipolar(end)], 'linewidth', 2);
title('双极性NRZ编码信号');
xlabel('时间');
ylabel('幅度');
axis([0 data_length -1.5 1.5]);

subplot(3, 1, 3);
stairs([0:data_length], [encoded_diff encoded_diff(end)], 'linewidth', 2);
title('差分编码信号');
xlabel('时间');
ylabel('幅度');
axis([0 data_length -0.5 1.5]);

figure(2);
subplot(3, 1, 1);
plot(t, modulated_2ASK, 'linewidth', 2);
title('2ASK 调制信号');
xlabel('时间');
ylabel('幅度');

subplot(3, 1, 2);
plot(t, modulated_2FSK, 'linewidth', 2);
title('2FSK 调制信号');
xlabel('时间');
ylabel('幅度');

subplot(3, 1, 3);
plot(t, modulated_2PSK, 'linewidth', 2);
title('2PSK 调制信号');
xlabel('时间');
ylabel('幅度');

figure(3);
subplot(3, 1, 1);
stairs([0:data_length], [received_2ASK received_2ASK(end)], 'linewidth', 2);
title('2ASK 解调信号');
xlabel('时间');
ylabel('幅度');
axis([0 data_length -0.5 1.5]);

subplot(3, 1, 2);
stairs([0:data_length], [received_2FSK received_2FSK(end)], 'linewidth', 2);
title('2FSK 解调信号');
xlabel('时间');
ylabel('幅度');
axis([0 data_length -0.5 1.5]);

subplot(3, 1, 3);
stairs([0:data_length], [received_2PSK received_2PSK(end)], 'linewidth', 2);
title('2PSK 解调信号');
xlabel('时间');
ylabel('幅度');
axis([0 data_length -0.5 1.5]);

% 创建一个文件夹来保存图片
output_folder = 'output_images';
if ~exist(output_folder, 'dir')
    mkdir(output_folder);
end

% 保存第一个图像
figure(1);
saveas(gcf, fullfile(output_folder, 'encoded_signals_no_noise.png'));

% 保存第二个图像
figure(2);
saveas(gcf, fullfile(output_folder, 'modulated_signals_no_noise.png'));

% 保存第三个图像
figure(3);
saveas(gcf, fullfile(output_folder, 'demodulated_signals_no_noise.png'));
