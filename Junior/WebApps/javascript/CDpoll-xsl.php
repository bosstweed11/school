<!DOCTYPE html>
<html>
<head>
<title>Select Your Favorite CD</title>
<link rel = "stylesheet" type = "text/css" href = "CDstyle.css"/>
<script type="text/javascript">
// CS 1520 Summer 2012 -- Compare to CDpoll-xml.php
// This has the same functionality as CDpoll-xml.php, but now using
// XSLT to process the resulting document.  Also, rather than receiving
// new rows in the table after an update, it receives the entire table
// in XML format, which is then processed using XSLT and transformed into
// a new HTML table.  The new HTML table then replaces the old one in the
// document. 

    // Initially we load the XSL stylesheet using AJAX.  When the 
    // stylesheet is returned we store it in a variable for future
    // reference.
    function getXSL()
    {
          var xhttp;
          if (window.XMLHttpRequest)
          {
                xhttp=new XMLHttpRequest();
          }
          else
          {
                xhttp=new ActiveXObject("Microsoft.XMLHTTP");
          }
          xhttp.open("GET","formatCDs.xsl",true);
          xhttp.onreadystatechange = function() { returnXSL(xhttp); };
          xhttp.send("");
    }

    function returnXSL(xhttp)
    {
          if (xhttp.readyState == 4)
          {
             if (xhttp.status == 200)
             {
                //var txt = xhttp.responseText;
                //alert(txt);
                xsl = xhttp.responseXML;
                // Once we have the xsl stylesheet, we can load
                // the CDs
                refreshPage();
             }
          }
    }

    // Send requests as in previous versions.  To more accurately describe
    // what is actually happening, the response function for type 2
    // requests has been changed to updateTable 
    function processData() {
        var httpRequest;
 
        var type = arguments[0];  // get type of call

        if (window.XMLHttpRequest) { // Mozilla, Safari, ...
            httpRequest = new XMLHttpRequest();
            if (httpRequest.overrideMimeType) {
                httpRequest.overrideMimeType('text/xml');
            }
        }
        else if (window.ActiveXObject) { // IE
            try {
                httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
                }
            catch (e) {
                try {
                    httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
                }
                catch (e) {}
            }
        }
        if (!httpRequest) {
            alert('Giving up :( Cannot create an XMLHTTP instance');
            return false;
        }
 
        var data;
        if (type == 1)
        {
            var choice = arguments[1];
            data = 'type=' + type + '&' + 'select=' + choice;  
            //alert(data);
        }
        else // type == 2
        {
            var rows = arguments[1];
            var title = arguments[2];
            var artist = arguments[3];
            data = 'type=' + type + '&rows=' + rows + '&title=' + title + '&artist=' + artist; 
            //alert(data);
        }

        httpRequest.open('POST', 'tabulate-xsl.php', true);
        httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

        if (type == 1)
              httpRequest.onreadystatechange = function() { showResults(httpRequest, choice); };
        else
              httpRequest.onreadystatechange = function() { updateTable(httpRequest); } ;
        httpRequest.send(data);
    }

    function processWritein()
    {
	var numrows = document.getElementById("theTable").rows.length-1;
        var title = document.pollForm.title.value;
        var artist = document.pollForm.artist.value;
        var ok = true;
        if (title == "")
        {
            alert("Please enter a title for your write-in vote");
            document.pollForm.title.focus();
            ok = false;
        }
        if (artist == "")
        {
            alert("Please enter an artist for your write-in vote");
            document.pollForm.artist.focus();
            ok = false;
        }
        if (ok)
        {
            document.pollForm.artist.value = "";
            document.pollForm.title.value = "";
            processData(2, numrows, title, artist);
        }
    }

    function showResults(httpRequest, choice)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
               // Get the root of the XML document
               var root = httpRequest.responseXML.documentElement;
               // Get the new count using the appropriate tag
               var newCount = root.getElementsByTagName('Count')[0].childNodes[0].nodeValue;
               //alert(newCount); 

               var T = document.getElementById("theTable");
               var R = T.rows[choice];
               var C = R.cells;
               var oldChild = C[2].childNodes[0];
               var txt = document.createTextNode(newCount);
               C[2].replaceChild(txt, oldChild);
               //C[2].innerHTML = newCount;
           }
           else
           {   alert('Problem with request'); }
       }
    }

    function updateTable(httpRequest)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
               //var txt = httpRequest.responseText;
               //alert(txt);
               // Get the XML document
               var root = httpRequest.responseXML;
               // Check how many CDs are in the response document.  If it
               // is none, don't do anything.  Otherwise, generate a new
               // table using xsl and replace the old table with the
               // new one.
               var newCDs = root.getElementsByTagName('CD').length;
               if (newCDs > 0)
	           {
                   var xsltProcessor=new XSLTProcessor();
	               xsltProcessor.importStylesheet(xsl);
                   // Get the new table using XSLT
	               var resultDocument = xsltProcessor.transformToFragment(root,document);
                   // Get the old table and then its parent.  Then replace
                   // the old table with the new one.
                   var theTable = document.getElementById("theTable");
                   var parent = theTable.parentNode;
                   parent.replaceChild(resultDocument,theTable);
                   window.status="Table updated at " + (new Date()).toString();
               }
               else
                   window.status = "";
           }
           else
           {   alert('Problem with request'); }
       }
    }

    function showSelected()
    {
          var rowind = arguments[0];
          var theTable = document.getElementById("theTable");
	  var theRow = theTable.rows[rowind];
          theRow.className = "highlight";
    }

    function makeRegular()
    {
          var rowind = arguments[0];
          var theTable = document.getElementById("theTable");
	      var theRow = theTable.rows[rowind];
          theRow.className = "regular";
    }

    function refreshPage()
    {
        var httpRequest;
 
        if (window.XMLHttpRequest) { // Mozilla, Safari, ...
            httpRequest = new XMLHttpRequest();
            if (httpRequest.overrideMimeType) {
                httpRequest.overrideMimeType('text/xml');
            }
        }
        else if (window.ActiveXObject) { // IE
            try {
                httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
                }
            catch (e) {
                try {
                    httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
                }
                catch (e) {}
            }
        }
        if (!httpRequest) {
            alert('Giving up :( Cannot create an XMLHTTP instance');
            return false;
        }
 
        // Since the server script is sending the entire table back
        // (rather than just the new rows) we can use the same callback
        // method for the refresh that we do for the write-in
        var type = 3; 
        var rows = document.getElementById("theTable").rows.length-1;
        var data = 'type=' + type + '&rows=' + rows;

        httpRequest.open('POST', 'tabulate-xsl.php', true);
        httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

        httpRequest.onreadystatechange = function() { updateTable(httpRequest); } ;
        httpRequest.send(data);
        t = setTimeout("refreshPage()", 60000);
    }
</script>
</head>
<body>
<center>
<script type="text/javascript">
   alert("Now loading the survey");
</script>
<form name = "pollForm">
<table id = "theTable" border = "1" class="thetable">
    <caption class="title">Select your Favorite CD<br/></caption>
    <tr align = "center">
        <th>Title</th><th>Artist</th><th>Count</th><th>Select</th>
    </tr>
</table>
<br /><center>
<h1>Write-in choice</h1>
<h2>
Title: <input type = "text" name = "title" value = ""><br />
Artist: <input type = "text" name = "artist" value = ""><br />
<input type = 'button' value = "Enter" onclick = 'processWritein()'>
<input type = "reset" name = "Reset">
</h2>
</form>
</center>
<script type="text/javascript">
    // Note that now we don't have any PHP in our original page.  All
    // of the CDs are loaded via AJAX -- even the initial table.
    var t, xsl;
    getXSL();
</script>
</body>
</html>
