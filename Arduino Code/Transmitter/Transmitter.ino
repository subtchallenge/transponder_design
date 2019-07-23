#define SWITCH_INPUT 4
#define LOOP_RATE 1

#define TEAM_NETWORK_ID 0x1234

// Function forward declarations.
int Update_NetworkID(unsigned int ID);
int Read_ATResponse();
int Broadcast_EStop();
void SendMessage(byte* Data, unsigned int Length);
byte Checksum(byte* Message, unsigned int Length);
void LED_Status(bool OK);
void LED_ConfigMode();

bool ConfigMode = false;

// Setup routine.
void setup()
{
  // Set up input pin for reading switch state.
  pinMode(SWITCH_INPUT, INPUT);
  // Set up red status LED as output pin.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize serial.
  Serial.begin(9600);

  

  // Check if booted up via USB power only (e.g. config mode).
  ConfigMode = digitalRead(SWITCH_INPUT) == LOW;

  // If in config mode, attempt to reconfigure the XBee once the switch goes on.
  if(ConfigMode)
  {
    // Wait until switch is turned on (need to turn XBee on)
    while(digitalRead(SWITCH_INPUT) == LOW)
    {
      LED_ConfigMode();
      delay(1000);
    }

    // Enter config mode to power XBee and remote configure.
    bool ConfigSuccess = true;

//    // Give PowerBoost and XBee enough time to boot up.
//    delay(100);
//
//    // Flush the input buffer.
//    while(Serial.available())
//    {
//      Serial.read();
//    }    
//
//    // If this point reached, the switch is now on.
//    bool ID_Updated = Update_NetworkID(TEAM_NETWORK_ID) == 0;
//
//    bool ConfigSuccess = ID_Updated;
//
//    // Indicate if a success or not.
//    LED_Status(ConfigSuccess);

    // If new configuration was successful, indicate to user to turn switch off.
    delay(1000);
    if(ConfigSuccess)
    {
      ConfigMode = false;
      while(digitalRead(SWITCH_INPUT) == HIGH)
      {
        LED_ConfigMode();
        delay(1000);
      } 
    }
    // Otherwise, never leave config mode because the XBEE may be in a partially configured state.
  }
}

// Loop routine.
void loop()
{
  // Wait until switch is turned on to broadcast.
  if(ConfigMode == false && digitalRead(SWITCH_INPUT) == HIGH)
  {
    // Switch is in the ON position.
    Broadcast_EStop();
  }

  delay(1000/LOOP_RATE);
}

// Function definitions.
int Update_NetworkID(unsigned int ID)
{
  // Create a data packet.
  byte* Data = new byte[6];
  Data[0] = 0x08;               // Frame Type: 0x08 AT Command
  Data[1] = 0x01;               // Frame ID: 1
  Data[2] = 0x49;               // AT Command: ID = 0x4944
  Data[3] = 0x44;               // AT Command: ID = 0x4944
  Data[4] = (ID >> 8) & 0xFF;   // ID high byte
  Data[5] = ID & 0xFF;          // ID low byte

  // Send the data packet.
  SendMessage(Data, 6);

  // Get the AT response.
  return Read_ATResponse();
}
void SendMessage(byte* Data, unsigned int Length)
{
  unsigned long FrameLength = Length + 4;
  // Create a new buffer to store the full frame packet.
  byte* Frame = new byte[FrameLength];
  // Set frame header.
  Frame[0] = 0x7E;
  // Set length bits.
  Frame[1] = (Length >> 8) & 0xFF;
  Frame[2] = Length & 0xFF;

  // Copy the data into the frame.
  for(int i = 0; i < Length; i++)
  {
    Frame[3 + i] = Data[i];
  }
  
  // Write the checksum.
  Frame[FrameLength - 1] = Checksum(Frame, FrameLength);

  // Write the frame to the serial port.
  Serial.write(Frame, FrameLength);

  // Delete the Frame and the Data arrays.
  delete [] Data;
  delete [] Frame;
}
int Read_ATResponse()
{
  // Read data from the buffer.
  unsigned long Length = 9;
  byte* Response = new byte[Length];
  
  int BytesRead = Serial.readBytes(Response, 9);

  // Check if the response timed out.
  if(BytesRead != Length)
  {
    delete [] Response;
    return -1;
  }

  // At this point, enough bytes received.  Verify checksum.
  if(Response[Length - 1] != Checksum(Response, Length))
  {
    delete [] Response;
    return -2;
  }

  // At this point, we have a valid packet.
  if(Response[3] != 0x88)
  {
    delete [] Response;
    return -3;
  }

  // At this point, we have a valid AT response packet.
  int Output = Response[7];
  delete [] Response;
    
  return Output;
}
int Broadcast_EStop()
{
  // Create a data packet.
  byte* Data = new byte[16];
  Data[0] = 0x17;               // Frame Type: 0x17 Remote AT Command
  Data[1] = 0x01;               // Frame ID: 1
  Data[2] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF (Broadcast)
  Data[3] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  Data[4] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  Data[5] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  Data[6] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  Data[7] = 0x00;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  Data[8] = 0xFF;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  Data[9] = 0xFF;               // 64b Address = 0x00 00 00 00 00 00 FF FF
  Data[10] = 0xFF;              // 16b Address = 0xFF FE (Reserved)
  Data[11] = 0xFE;              // 16b Address = 0xFF FE (Reserved)
  Data[12] = 0x03;              // Remote CMD Options = 0b11 (Disable ACK, Apply Changes)
  Data[13] = 0x44;              // AT Command: D1 = 0x4431
  Data[14] = 0x31;              // AT Command: D1 = 0x4431
  Data[15] = 0x05;              // Param Value: Digital Output (High) = 0x05

  // Send the data packet.
  SendMessage(Data, 16);
}
byte Checksum(byte* Message, unsigned int Length)
{
  // Formula: Add all bytes (except for start delimiter and length), keep only the lowest 8 bits, and subtract of 0xFF.
  unsigned long Sum = 0;
  for(unsigned int i = 3; i < Length - 1; i++)
  {
    Sum += Message[i];
  }
  return (byte)(255-(Sum % 256));
}

void LED_Status(bool OK)
{
  if(OK)
  {
    for(unsigned int i = 0; i < 3; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
  else
  {
    for(unsigned int i = 0; i <2; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }
}
void LED_ConfigMode()
{
  for(unsigned int i = 0; i < 5; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
}
