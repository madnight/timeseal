# timeseal
Chess timeseal exploit

Zero time lag Modification:

![alt text](https://asmrev.files.wordpress.com/2014/06/untitled-12.jpg?w=855 )


Random time lag Modification: 
<pre><code>
00401421   E8 CA0D0000      CALL timeseal.004021F0  
00401426   25 FF030000      AND EAX,3FF  
0040142B   05 01010000      ADD EAX,101  
00401430   8B15 E0A14000    MOV EDX,DWORD PTR DS:[40A1E0]  
00401436   03D0             ADD EDX,EAX  
00401438   8915 E0A14000    MOV DWORD PTR DS:[40A1E0],EDX  
0040143E   8BC2             MOV EAX,EDX  
</code></pre>


This hook is hardcoded with Ollydbg into the timeseal.exe used by Winboard.  
Timeseal usually calculates the Lag via two Timestamps.  
I modified the Timestamps by adding an Random Offset (CALL timeseal.004021F0 := rand()).  
AND EAX,3FF is our asm Modulo Function rand()%1024 + fixed offset hex 101.  

Download: http://www.file-upload.net/download-9041753/timeseal.exe.html  
Mirror: http://www.share-online.biz/dl/XAPVA07NLXKE  

* You cant loose on time (even under 0 seconds)
* You will be always faster than your opponent
* It its not so obvious because of the random offset, opponents think that you are lagging
* It is also possible to hook into timeseal via DLL.

Blog: https://asmrev.wordpress.com/
