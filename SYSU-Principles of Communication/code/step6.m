% 初始化
clc;
clear;

% 读取图像
img = imread('lena.bmp');
imgSize = size(img);
disp('Original Image Size:');
disp(imgSize);

% 将图像转换为二进制数据
imgData = de2bi(img(:), 8, 'left-msb'); % 转换为二进制数据
imgData = imgData(:);

% 显示二进制数据大小
disp('Binary Image Data Size:');
disp(size(imgData));

% 编码
encodedImgData = imgData;

% 调制（例如2ASK）
M = 2; % 2ASK
modImgData = pskmod(encodedImgData, M);

% 添加不同强度的高斯白噪声
snrLevels = [5, 10, 15]; % 不同的SNR值
for snr = snrLevels
    % 添加噪声
    noisyImgSignal = awgn(modImgData, snr, 'measured');
    
    % 解调
    demodImgData = pskdemod(noisyImgSignal, M);
    
    % 解码
    receivedImgData = demodImgData;
    
    % 显示接收到的二进制数据大小
    disp(['Received Binary Image Data Size at SNR ', num2str(snr), ':']);
    disp(size(receivedImgData));
    
    % 检查接收的数据是否与发送的数据大小一致
    if length(receivedImgData) ~= length(imgData)
        error('Received data size does not match original data size.');
    end
    
    % 转换回图像格式
    receivedImg = reshape(bi2de(reshape(receivedImgData, [], 8), 'left-msb'), imgSize);
    receivedImg = uint8(receivedImg); % 确保数据类型为uint8
    imwrite(receivedImg, ['output_images/received_snr_' num2str(snr) '.bmp']);
    
    % 显示接收的图像大小
    disp(['Received Image Size at SNR ', num2str(snr), ':']);
    disp(size(receivedImg));
    
    % 计算误差
    imgDiff = im2double(img) - im2double(receivedImg); % 使用im2double转换为浮点数进行差异计算
    numImgErrors = nnz(imgDiff);
    disp(['Number of image pixel errors at SNR ' num2str(snr) ': ', num2str(numImgErrors)]);
end
