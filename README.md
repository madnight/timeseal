# timeseal
Chess timeseal exploit, time lag modification

![alt text](https://asmrev.files.wordpress.com/2014/06/untitled-12.jpg?w=855 )


Random time lag modification: 
<pre><code>
00401421   E8 CA0D0000      CALL timeseal.004021F0  
00401426   25 FF030000      AND EAX,3FF  
0040142B   05 01010000      ADD EAX,101  
00401430   8B15 E0A14000    MOV EDX,DWORD PTR DS:[40A1E0]  
00401436   03D0             ADD EDX,EAX  
00401438   8915 E0A14000    MOV DWORD PTR DS:[40A1E0],EDX  
0040143E   8BC2             MOV EAX,EDX  
</code></pre>


This hook is hardcoded with OllyDbg into the timeseal binary. Timeseal usually calculates the lag via two timestamps. The timestamps are modified by adding an random offset (CALL timeseal.004021F0 := rand()), while AND EAX,3FF is the asm modulo function rand()%1024 + fixed offset hex 101.  

Features:

* You cant loose on time
* You will be always faster than your opponent
* It seems legit, opponents think that you are lagging

This exploit has been reported serval times to chess servers that do allow clientside timeseal, but some chess servers just dont care.
