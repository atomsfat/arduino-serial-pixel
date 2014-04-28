import processing.serial.*;

// The serial port:
Serial myPort;       

String val;
boolean firstContact = false;

// List all the available serial ports:
//println(Serial.list());

int index;
int red;
int blue;
int greee;

void setup() 
{
  size(200, 200);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  //String portName = Serial.list()[0];
 //  String portName = "/dev/tty.HB01-DevB";
  String portName = "/dev/ttyUSB0";
  myPort = new Serial(this, portName, 115200);
}


void draw() { 
}

String getRandom(int max, int length){
  int number = int(random(max));  
  String str = "" + number;
  while(str.length() < length){
   str = "0" + str; 
  }
 return str;
}

void serialEvent(Serial myPort){
        
  println("====>>>");
  val = myPort.readStringUntil('\n');
  if( val != null){
      val = trim(val);
      println("===retrived===");
      println(val);
      println("=============");
    
    if (firstContact == false){  
      if(val.equals("A")){
        myPort.clear();
        firstContact = true;
        
        println("contact");
        String random = "" + getRandom(20, 2) + getRandom(255, 3) + getRandom(255, 3) + getRandom(255, 3) + "\n"; 
        for(char ch : random.toCharArray()){
          myPort.write(ch);            
        } 
      }
    }else{
      if(val.equals("more")){
        String random = "" + getRandom(20, 2) + getRandom(255, 3) + getRandom(255, 3) + getRandom(255, 3) + "\n"; 
        for(char ch : random.toCharArray()){
          myPort.write(ch);            
        }    
      }     
    }
  }
}
