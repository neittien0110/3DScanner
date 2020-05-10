import cv2
import serial

# Web Cam Index, from 0 to next
DEVICE_INDEX = 1 
# Com port name, connect to Arduino board 
COM_PORT = 'COM12'
# Title of the Video dialog
FORM_NAME_VIDEO = 'Object in 3D Scanner'



ser = serial.Serial(COM_PORT, 9600, parity=serial.PARITY_NONE)  # open serial port, none-blocking
print(ser.name)         # check which port was really used

cv2.namedWindow(FORM_NAME_VIDEO)
cv2.VideoCapture
vc = cv2.VideoCapture(DEVICE_INDEX)

if vc.isOpened(): # try to get the first frame
    rval, frame = vc.read()
else:
    rval = False
    print('Cannot open the camera') 
    
while rval:
    cv2.imshow(FORM_NAME_VIDEO, frame)
    rval, frame = vc.read()
    key = cv2.waitKey(20)
    if key == 27: # exit on ESC
        break
    image_index = int(ser.readline())
    print(image_index)
    # build image's file name 
    img_name = "W:\\workspaces\\Arduino\\3DScanner\\sources\\ImageCapturingApp_PCver\\output\\object_{}.png".format(image_index)
    # save image 
    cv2.imwrite(img_name, frame)        
    print(img_name)

cv2.destroyWindow(FORM_NAME_VIDEO)