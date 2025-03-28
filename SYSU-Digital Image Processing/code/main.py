import cv2
import numpy as np
import math

def he(image):
    B, G, R = cv2.split(image)
    B = cv2.equalizeHist(B)
    G = cv2.equalizeHist(G)
    R = cv2.equalizeHist(R)
    result = cv2.merge((B, G, R))
    return result

def clahe(image,clipLimit=2.0, tileGridSize=(8, 8)):
    B,G,R = cv2.split(image)
    clahe = cv2.createCLAHE(clipLimit, tileGridSize)
    clahe_B = clahe.apply(B)
    clahe_G = clahe.apply(G)
    clahe_R = clahe.apply(R)
    result = cv2.merge((clahe_B,clahe_G,clahe_R))
    cv2.imshow('clahe', result)
    return result

# ACE
def stretchImage(data, s=0.005, bins=2000):
    ht = np.histogram(data, bins);
    d = np.cumsum(ht[0]) / float(data.size)
    lmin = 0;
    lmax = bins - 1
    while lmin < bins:
        if d[lmin] >= s:
            break
        lmin += 1
    while lmax >= 0:
        if d[lmax] <= 1 - s:
            break
        lmax -= 1
    return np.clip((data - ht[1][lmin]) / (ht[1][lmax] - ht[1][lmin]), 0, 1)


# 根据半径计算权重参数矩阵
g_para = {}


def getPara(radius=5):
    global g_para
    m = g_para.get(radius, None)
    if m is not None:
        return m
    size = radius * 2 + 1
    m = np.zeros((size, size))
    for h in range(-radius, radius + 1):
        for w in range(-radius, radius + 1):
            if h == 0 and w == 0:
                continue
            m[radius + h, radius + w] = 1.0 / math.sqrt(h ** 2 + w ** 2)
    m /= m.sum()
    g_para[radius] = m
    return m


# 常规的ACE实现
def zmIce(I, ratio=4, radius=300):
    para = getPara(radius)
    height, width = I.shape
    zh = []
    zw = []
    n = 0
    while n < radius:
        zh.append(0)
        zw.append(0)
        n += 1
    for n in range(height):
        zh.append(n)
    for n in range(width):
        zw.append(n)
    n = 0
    while n < radius:
        zh.append(height - 1)
        zw.append(width - 1)
        n += 1
    # print(zh)
    # print(zw)

    Z = I[np.ix_(zh, zw)]
    res = np.zeros(I.shape)
    for h in range(radius * 2 + 1):
        for w in range(radius * 2 + 1):
            if para[h][w] == 0:
                continue
            res += (para[h][w] * np.clip((I - Z[h:h + height, w:w + width]) * ratio, -1, 1))
    return res


# 单通道ACE快速增强实现
def zmIceFast(I, ratio, radius):
    #print(I)
    height, width = I.shape[:2]
    if min(height, width) <= 2:
        return np.zeros(I.shape) + 0.5
    Rs = cv2.resize(I, (int((width + 1) / 2), int((height + 1) / 2)))
    Rf = zmIceFast(Rs, ratio, radius)  # 递归调用
    Rf = cv2.resize(Rf, (width, height))
    Rs = cv2.resize(Rs, (width, height))

    return Rf + zmIce(I, ratio, radius) - zmIce(Rs, ratio, radius)


# rgb三通道分别增强 ratio是对比度增强因子 radius是卷积模板半径
def zmIceColor(I, ratio=4, radius=3):
    res = np.zeros(I.shape)
    for k in range(3):
        res[:, :, k] = stretchImage(zmIceFast(I[:, :, k], ratio, radius))
    return res

# 光照校正
def Adaptive_light_correction(img):
    height = img.shape[0]
    width = img.shape[1]

    HSV_img = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    V = HSV_img[:,:,2]

    kernel_size = min(height, width)
    if kernel_size % 2 == 0:
        kernel_size -= 1

    SIGMA1 = 15
    SIGMA2 = 80
    SIGMA3 = 250
    q = np.sqrt(2.0)
    F = np.zeros((height,width,3),dtype=np.float64)
    F[:,:,0] = cv2.GaussianBlur(V,(kernel_size, kernel_size),SIGMA1 / q)
    F[:,:,1] = cv2.GaussianBlur(V,(kernel_size, kernel_size),SIGMA2 / q)
    F[:,:,2] = cv2.GaussianBlur(V,(kernel_size, kernel_size),SIGMA3 / q)
    F_mean = np.mean(F,axis=2)
    average = np.mean(F_mean)
    gamma = np.power(0.5,np.divide(np.subtract(average,F_mean),average))
    out = np.power(V/255.0,gamma)*255.0
    HSV_img[:,:,2] = out
    img = cv2.cvtColor(HSV_img,cv2.COLOR_HSV2BGR)
    return img

# 暗通道去雾
def zmMinFilterGray(src, r=7):
    #最小值滤波, r是滤波器半径
    return cv2.erode(src, np.ones((2 * r + 1, 2 * r + 1)))

def guidedfilter(I, p, r, eps):
    height, width = I.shape
    m_I = cv2.boxFilter(I, -1, (r, r))
    m_p = cv2.boxFilter(p, -1, (r, r))
    m_Ip = cv2.boxFilter(I * p, -1, (r, r))
    cov_Ip = m_Ip - m_I * m_p

    m_II = cv2.boxFilter(I * I, -1, (r, r))
    var_I = m_II - m_I * m_I

    a = cov_Ip / (var_I + eps)
    b = m_p - a * m_I

    m_a = cv2.boxFilter(a, -1, (r, r))
    m_b = cv2.boxFilter(b, -1, (r, r))
    return m_a * I + m_b


def Defog(m, r, eps, w, maxV1):                 # 输入rgb图像，值范围[0,1]
    '''计算大气遮罩图像V1和光照值A, V1 = 1-t/A'''
    V1 = np.min(m, 2)                           # 得到暗通道图像
    Dark_Channel = zmMinFilterGray(V1, 7)
    cv2.imshow('Dark', Dark_Channel)    # 查看暗通道
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    V1 = guidedfilter(V1, Dark_Channel, r, eps)  # 使用引导滤波优化
    bins = 2000
    ht = np.histogram(V1, bins)                  # 计算大气光照A
    d = np.cumsum(ht[0]) / float(V1.size)
    for lmax in range(bins - 1, 0, -1):
        if d[lmax] <= 0.999:
            break
    A = np.mean(m, 2)[V1 >= ht[1][lmax]].max()
    V1 = np.minimum(V1 * w, maxV1)               # 对值范围进行限制
    return V1, A


def deHaze(m, r=81, eps=0.001, w=0.95, maxV1=0.80, bGamma=False):
    Y = np.zeros(m.shape)
    Mask_img, A = Defog(m, r, eps, w, maxV1)             # 得到遮罩图像和大气光照

    for k in range(3):
        Y[:,:,k] = (m[:,:,k] - Mask_img)/(1-Mask_img/A)  # 颜色校正
    Y = np.clip(Y, 0, 1)
    if bGamma:
        Y = Y ** (np.log(0.5) / np.log(Y.mean()))       # gamma校正,默认不进行该操作
    return Y

if __name__ == '__main__':
    img = cv2.imread('111/554.jpg')
    res_1 = deHaze((img) / 255.0) * 255.0 # 暗通道去雾
    res_1 = res_1.astype(np.uint8) # 类型转换
    res_2 = Adaptive_light_correction(res_1) # 光照矫正
    res_3 = zmIceColor(res_2 / 255.0) * 255.0 # ACE
    cv2.imshow("initial_img", img)
    cv2.imshow("deHaze", res_1)
    cv2.imshow("light", res_2)
    cv2.imshow("ace", res_3)
    cv2.imwrite("33/5015.jpg", res_3);
    cv2.waitKey(0)
    cv2.destroyAllWindows()

