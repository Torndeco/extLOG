## Arma3 Extension Logger + Console Window (windows  only atm)

Its a simple logger with support for output to multiple files + GUI Console Window.


#### Syntax
```
<Colour ID>:<Log Filename>:<Message>
```  
```
<Colour ID>::<Message>
```  

Colour ID 0 = NO OUTPUT TO GUI CONSOLE  
Colour ID 1-9 = GUI CONSOLE OUTPUT, colour is defined in extLOG.ini  

#### Examples
```
"extLOG" callExtension "1:SERVER:Starting";
```
Output message Starting to GUI Console  
Output message to logfile SERVER.log.  


```
"extLOG" callExtension "1::Starting";
```
Output message Starting to GUI Console only.  

```
"extLOG" callExtension "0:SERVER:Starting";
```
Output message Starting to logfile SERVER.log only.  


#### Download at
https://github.com/Torndeco/extLOG/releases


#### Windows Requirements  
Windows Server 2008 + Later  
Windows 7 + Later  

Install vcredist_x86.exe  
http://www.microsoft.com/en-ie/download/details.aspx?id=40784  

#### Donation Link @  

https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=2SUEFTGABTAM2
 
