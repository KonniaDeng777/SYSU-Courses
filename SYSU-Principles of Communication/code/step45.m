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

% 解调
demodImgData = pskdemod(modImgData, M);

% 解码
receivedImgData = demodImgData;

% 显示接收到的二进制数据大小
disp('Received Binary Image Data Size:');
disp(size(receivedImgData));

% 检查接收的数据是否与发送的数据大小一致
if length(receivedImgData) ~= length(imgData)
    error('Received data size does not match original data size.');
end

% 转换回图像格式
receivedImg = reshape(bi2de(reshape(receivedImgData, [], 8), 'left-msb'), imgSize);
receivedImg = uint8(receivedImg);
imwrite(receivedImg, 'output_images/received.bmp');

% 显示接收的图像大小
disp('Received Image Size:');
disp(size(receivedImg));

% 计算误差
imgDiff = img - receivedImg;
numImgErrors = nnz(imgDiff);
disp(['Number of image pixel errors: ', num2str(numImgErrors)]);
