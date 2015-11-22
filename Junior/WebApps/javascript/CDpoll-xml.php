<!DOCTYPE html>
<html>
<head>
<title>Select Your Favorite CD</title>
<link rel = "stylesheet" type = "text/css" href = "CDstyle.css"/>
<script type="text/javascript">
// CS 1520 Summer 2012 -- Compare to CDpoll-update.php
// This has the same functionality as CDpoll-update.php, but now using
// an XML document as the response.  Most previous comments have been
// removed.  See more new comments below.

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

        httpRequest.open('POST', 'tabulate-xml.php', true);
        httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

        if (type == 1)
              httpRequest.onreadystatechange = function() { showResults(httpRequest, choice); };
        else
              httpRequest.onreadystatechange = function() { addNewRows(httpRequest, title, artist); } ;
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

    function addNewRows(httpRequest, title, artist)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
               // Get the root of the XML document
               var root = httpRequest.responseXML.documentElement;
               // Check the type of the return value.  If the type is "Ack"
               // simply add the new row that the user entered.  Otherwise,
               // parse the XML document for the new rows and add them to
               // the HTML table
               var rettype = root.getElementsByTagName('Type')[0].childNodes[0].nodeValue;
               if (rettype == "Ack")
               {
		   addRow(title, artist, '1');
               }
               else
	       {
                   var newRows = root.getElementsByTagName('CD');
		   for (var i = 0; i < newRows.length; i++)
	           {
			var theRow = newRows[i];
                        var theTitle = theRow.getElementsByTagName('Title')[0].childNodes[0].nodeValue;
                        var theArtist = theRow.getElementsByTagName('Artist')[0].childNodes[0].nodeValue;
                        var theVotes = theRow.getElementsByTagName('Votes')[0].childNodes[0].nodeValue;
			addRow(theTitle, theArtist, theVotes);
		   }
		   addRow(title, artist, '1');
	       }
           }
           else
           {   alert('Problem with request'); }
       }
    }

    function updateRows(httpRequest)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
               // Again check the return type.  If it is "Update", there are
               // new rows to add, so we add them (same parsing as done
               // above in addNewRows).  Otherwise, we make not changes.
               var root = httpRequest.responseXML.documentElement;
               var rettype = root.getElementsByTagName('Type')[0].childNodes[0].nodeValue;
               if (rettype == "Update")
               {
                   var newRows = root.getElementsByTagName('CD');
                   for (var i = 0; i < newRows.length; i++)
                   {
                        var theRow = newRows[i];
                        var theTitle = theRow.getElementsByTagName('Title')[0].childNodes[0].nodeValue;
                        var theArtist = theRow.getElementsByTagName('Artist')[0].childNodes[0].nodeValue;
                        var theVotes = theRow.getElementsByTagName('Votes')[0].childNodes[0].nodeValue;
                        addRow(theTitle, theArtist, theVotes);
                   }
                   window.status="Table updated at " + (new Date()).toString();
               }
               else
               {
                  window.status="";
               }
          }
          else
          {   alert('Problem with request'); }
       }
    }
 
    // The mechanics of adding a row does not change at all in this
    // version.  The only difference is the parsing of the input as
    // XML to set up the data.  Thus, the function below has not changed.
    function addRow(title, artist, vote)
    {
          var T = document.getElementById("theTable");
          var len = T.rows.length;
          var R = T.insertRow(len); 
          R.align = 'center';       
          R.className = 'regular';

          var C = R.insertCell(0);  
          var txt = document.createTextNode(title);
          C.appendChild(txt);
          C = R.insertCell(1);
          txt = document.createTextNode(artist);
          C.appendChild(txt);
          C = R.insertCell(2);
          txt = document.createTextNode(vote);
          C.appendChild(txt);

          C = R.insertCell(3);     
          var rb = document.createElement('input');
          rb.setAttribute('type', 'radio');
          rb.setAttribute('name', 'options');
          rb.setAttribute('value', len);
          rb.onclick = function() { processData(1, len); };
          rb.onmouseover = function() { showSelected(len); };
          rb.onmouseout = function() { makeRegular(len); };
          C.appendChild(rb);
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

    function startRefresh()
    {
	 t = setTimeout("refreshPage()", 60000);
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
 
        var type = 3; 
        var rows = document.getElementById("theTable").rows.length-1;;
        var data = 'type=' + type + '&rows=' + rows;

        httpRequest.open('POST', 'tabulate-xml.php', true);
        httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

        httpRequest.onreadystatechange = function() { updateRows(httpRequest); } ;
        httpRequest.send(data);
        t = setTimeout("refreshPage()", 60000);
    }
</script>
</head>
<body onload = "startRefresh()">
<center>
<script type="text/javascript">
   alert("Now loading the survey");
</script>
<form name = "pollForm">
<table id = "theTable" border = "1" class="thetable">
    <caption class="title">Select your Favorite CD<br/></caption>
    <tr align = center>
        <th>Title</th><th>Artist</th><th>Count</th><th>Select</th>
    </tr>
<?php
    $passlist = file("password.php");
    $passwd = rtrim($passlist[1]);
    $passwd = preg_replace('/#/','',$passwd);
    $db = mysql_connect('localhost', 'nomad', "$passwd");
    mysql_select_db('nomad') or die("Bogus");
    $result = mysql_query("select id, Title, Artist, Votes from CDs");
    $rows = mysql_num_rows($result);
    for ($ctr = 1; $ctr <= $rows; $ctr++):
        echo "<tr align = 'center' class = 'regular'>";
        $row = mysql_fetch_array($result);
        $id = $row["id"];
        $title = $row["Title"];
        $artist = $row["Artist"];
        $votes = $row["Votes"];
        echo "<td>$title</td>";
        echo "<td>$artist</td>";
        echo "<td>$votes</td>";
        echo "<td><input type = 'radio' name = 'options' 
                       value = '$id'
                       onclick = 'processData(1, $id)'
                       onmouseover = 'showSelected($id)'
                       onmouseout = 'makeRegular($id)'</td>";
        echo "</tr>";
    endfor;
?>
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
    var win, t;
</script>
</body>
</html>
