import processing.serial.*;
Serial SerialPort;
PrintWriter output;
 
void setup() {
   String portName = Serial.list()[1]; // 번호를 변경해 주면서 포트번호를 찾아준다
   println(Serial.list());
   SerialPort = new Serial( this, portName, 9600 ); // 아두이노의 보레이트에 맞춘다
   output = createWriter( "/data.txt" ); // 파일이 저장되는 곳을 설정한다 
}
 
void draw()
{
    // 시리얼 포트의 데이터 확인
    int temp = SerialPort.read();
    if( temp != -1 ) {
         int value = temp;
         println(value);
         output.write(value);
         
    }
}
 
void keyPressed() {
    // 텍스트 파일을 완성시키기 
    output.flush();  // Writes the remaining data to the file
    output.close();  // Finishes the file
    exit();  // Stops the program
}
