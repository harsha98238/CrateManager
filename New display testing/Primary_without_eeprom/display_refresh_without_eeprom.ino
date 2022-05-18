void displayCount()
{
  char tempBuffer[30];
  
  sprintf(tempBuffer,"%-6d",color1Cnt);
  String colour1Temp = tempBuffer;
  sprintf(tempBuffer,"%-6d",color2Cnt);
  String colour2Temp = tempBuffer;
  sprintf(tempBuffer,"%-6d",color3Cnt);
  String colour3Temp = tempBuffer;
  sprintf(tempBuffer,"%-6d",FullCopCount);
  String FullCopCountTemp = tempBuffer;
  sprintf(tempBuffer,"%-6d",krichiCnt);
  String krichiCntTemp = tempBuffer;
  sprintf(tempBuffer,"%-6d",BinSize);
  String BinSizeTemp = tempBuffer;
  String L1Count = "TB1="+colour1Temp+"TB2="+colour2Temp;
  String L2Count = "RB1="+FullCopCountTemp+"RB2="+krichiCntTemp;
 
  String L3Count = "TB3="+colour3Temp+"BS ="+BinSizeTemp;

  String L4Count =EmptyBuffer;
  displayMenu(L1Count,L2Count,L3Count,L4Count);
//  color2Cnt +=1;
//  color3Cnt +=1;
//  FullCopCount +=1;
//  krichiCnt +=1;
//  color1Cnt +=1;
}
void displayMenu(String line0,String line1,String line2,String line3){


   DynamicJsonDocument doc(200);
   doc["L0"] = line0;
   doc["L1"] = line1;
   doc["L2"] = line2;
   doc["L3"] = line3;
   
   String message ;
   serializeJsonPretty(doc, message);
   Serial1.println(message);
   Serial.println(message);
   

  
}
