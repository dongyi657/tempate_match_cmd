# tempate_match_cmd
 环境QT opencv3.4.6
.\template_match_cmd.exe 3 C:\Users\H237428\Pictures\src.JPG C:\Users\H237428\Pictures\temp2.JPG

tempate_match_cmd.exe 
 @方法1、3、5
 @原图像
 @查找图像
 @返回QString
 匹配个数
 (第一个匹配图像点位置)匹配极小值，匹配极大值

  本工具图片匹配使用函数是opencv中的matchTemplate
取最佳匹配值是用minMaxLoc
但是minMaxLoc是取极值，而不是所有匹配
该工具minMaxLoc循环取极值并记录。
