#include <Arduino.h>
#include <FS.h> //manage file system

// File function: isDirectory(),name(),size()
// File root = fs.open(dirname,"r"); mở file theo directory (fs là file system SD)
// File file = root.openNextFile(); mở file con
// file.close(); đóng file

// while(file.available()){         đọc file
// Serial.write(file.read());
// }

// File file = fs.open(dirname,"w")     viết file
// file.print(variable)

// fs.mkdir(path); tạo directory mới, return true nếu thành công
// fs.rmdir(path); xóa directory
// fs.rename(path1, path2)
// fs.remove(path)


// listDir(SD, "/", 0); mở listdir file root
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);
  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }
  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    }
    else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
