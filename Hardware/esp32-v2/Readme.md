# Extending support for other signals

1. Create variable to store signal data
   ```c++
   String NewData = "[";
   ```
2. Add to switch case block. **Add before `default` case**
   ```c++
   void readBuffer(HardwareSerial buffReader){
       ...
       switch (rx) {
       case 'N':
           active = true;
           combineTo = &NewData;
           break;
       ...
   }
   ```
3. Add to the HTTPpayload and add the removeComma statement in `Send_data` function

   ```c++
   void Send_data(WiFiClient cl) {
        ...
        removeComma(NewData);
        ...
        payload = ... + NewData + "] ," ...;
   }

   ```

4. Add to `resetData`
   ```c++
   void resetData(){
       ...
       NewData = "[";
   }
   ```
