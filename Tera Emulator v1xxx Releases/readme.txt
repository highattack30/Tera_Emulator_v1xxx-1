to start server you need
- a mysql server on ip 127.0.0.1:3306
- a web server on ip 127.0.0.1:80

[download and install xampp]
 -go to c:/xampp/htdocs and delete all in that foalder
 -go to web->svr->serverlist->serverlist.en and change 192.168.0.11 to your local ip and 0.0.0.0 to your global ip [wan ip]
 -copy all from web folder to htdocs folder 
 -start xampp control panel [c://xampp//xampp-control.exe]
      -start apache
      -start mysql

 -open browser and go to 127.0.0.1/phpmyadmin
	-go to 'New' [on the left] and create 'tera_q' database
	-select tera_q database
	-go to 'Import' tab [top side of web page]
	-click 'Choose File' and choose db.sql file found in server bin folder
        -if it didn't succeeded, you have to use Navicat [you can find tutorials online, 'how to connect to database with navicat','how to execute sql file on database with navicat']
 -go to server.txt in Config folder and change the 192.168.0.11 to your local ip.
 -go to luncher folder and open generator, replace all text with 'http://%yourlocal/global ip%/svr/serverlist/serverlist.en' and click Generate
 -copy all from luncher folder except generator.exe to TERA client folder. [lateset NA Tera client, haven't tested the Eu Tera client yet]
 -start Tera Emulator v1030 -> ./start
 -start tera through teraLuncher.exe 'admin','admin'

