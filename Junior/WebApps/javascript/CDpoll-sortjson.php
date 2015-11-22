<!DOCTYPE html>
<html>
<head>
<title>Select Your Favorite CD</title>
<link rel = "stylesheet" type = "text/css" href = "CDstyle.css"/>
<script type="text/javascript">
// CS 1520 Summer 2012 -- Compare to CDpoll-sortxml.php
// This has the same functionality as CDpoll-sortxml.php, but now
// the return data is JSON encoded.

// See more comments below

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
            var id = arguments[2];
            data = 'type=' + type + '&' + 'select=' + id;  
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

        httpRequest.open('POST', 'tabulate-json.php', true);
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
               // Get the JSON encoded text, then parse it using JSON.parse
               // Once it is parsed we can access the data using object
               // notation.  After that the updating to the table is the same
               // as before.
               // (note: this will not work for IE prior to IE8, since JSON
               // does not exist for ealier IE versions.  If you are using
               // IE 7 or below, UPGRADE!)
               var data = httpRequest.responseText;
               var newData = JSON.parse(data);
               var newCount = newData.Contents;

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

    // We are still adding CDs to our array of objects, but we are now
    // building them in a slightly different way.  In particular, we
    // parse the JSON data to get an array of "objects" (generic).  We then
    // iterate through that array to create pseudo-type CD objects, using
    // our CD constructor.  Compare this with CDpoll-sortxml.php to see
    // how it is the same and how it is different.
    function addNewRows(httpRequest, title, artist)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
               var data = httpRequest.responseText;
               //alert(data);
               // Parse the JSON data
               var newData = JSON.parse(data);
               var rettype = newData.Type;
               if (rettype == "Ack")
               {
		   addRowToList(null, title, artist, '1');
               }
               else
	       {
                   // The Contents field is an array containing all of the
                   // CDs that we need to add to our table.  Iterate through
                   // that array, making a CD object out of each generic
                   // object.  Note how much simpler the syntax is here than
                   // with the XML data.
                   var newRows = newData.Contents;
		   for (var i = 0; i < newRows.length; i++)
	           {
			var theRow = newRows[i];
			addRowToList(theRow.id, theRow.Title, theRow.Artist, theRow.Votes);
		   }
		   addRowToList(null, title, artist, '1');
	       }
               showCDTable();
           }
           else
           {   alert('Problem with request'); }
       }
    }

    // Same idea and parsing as for addNewRows above.  See that function for
    // comments.
    function updateRows(httpRequest)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
               var data = httpRequest.responseText;
               //alert(data);
               var newData = JSON.parse(data);
               var rettype = newData.Type;
               if (rettype == "Update")
               {
                   var newRows = newData.Contents;
                   for (var i = 0; i < newRows.length; i++)
                   {
                        var theRow = newRows[i];
                        addRowToList(theRow.id, theRow.Title, theRow.Artist, theRow.Votes);
                   }
                   showCDTable();
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

    // Create a CD
    function CD(id, title, artist, votes)
    {
         this.id = id;
         this.title = title;
         this.artist = artist;
         this.votes = votes;
    }

    // Compare CDs by name
    function by_name(a, b)
    {
	 if (a.title < b.title) return -1;
         else if (a.title == b.title) return 0;
         else return 1;
    }

    // Add a new row to the CD array.  If the id is null it means that
    // the CD was a write-in (not sent back from the server).  We then give
    // it the appropriate id (assuming the DB uses auto-increment).  As
    // stated above, it would be better if we received this new CD from the
    // server just so we would make sure to get the correct id.
    function addRowToList(id, title, artist, votes)
    {
         var currCD;
         if (id == null)
             currCD = new CD(CDcount+1, title, artist, votes);
         else
             currCD = new CD(id, title, artist, votes);
         theCDs[CDcount] = currCD;
         CDcount++;
    }

    // Sort the CDs then generate a new table, replacing the old one with
    // the new.  Note how the various attributes are generated using DOM.
    function showCDTable()
    {
         theCDs.sort(by_name);
         var T = document.getElementById("theTable");
         var tParent = T.parentNode;

         var newT = document.createElement('table');
         newT.setAttribute('id', 'theTable');
         newT.border = 1;
         newT.className = 'thetable';
         var cap = newT.createCaption();
         var contents = document.createTextNode('Select Your Favorite CD');
         cap.appendChild(contents);
         cap.className = "title";
         var hrow = newT.insertRow(0);
         hrow.align = 'center';

         var currCell = hrow.insertCell(0);
         contents = document.createTextNode('Title');
         currCell.appendChild(contents);

         var currCell = hrow.insertCell(1);
         contents = document.createTextNode('Artist');
         currCell.appendChild(contents);

         var currCell = hrow.insertCell(2);
         contents = document.createTextNode('Count');
         currCell.appendChild(contents);

         var currCell = hrow.insertCell(3);
         contents = document.createTextNode('Select');
         currCell.appendChild(contents);

         // New table is set up, but without CD rows.  Replace old with new
         // then add the CDs
         tParent.replaceChild(newT, T);

         for (var i = 0; i < CDcount; i++)
         {
             addRow(theCDs[i].id, theCDs[i].title, theCDs[i].artist, theCDs[i].votes);
         }
    }
 
    // When adding a row we now have to distinguish between the row index
    // and the CD id.  The index is used for selection and for highlighting,
    // while the id is used to update the count via AJAX.
    function addRow(id, title, artist, vote)
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
          rb.setAttribute('value', id);
          rb.onclick = function() { processData(1, len, id); };
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

    // We now refresh the page immediately to load the initial CDs
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
        // Special case:  Before initial table is displayed ther are
        // no rows.  We don't want the submitted variable to be negative,
        // however.
        if (rows == -1)
	    rows = 0;
        var data = 'type=' + type + '&rows=' + rows;

        httpRequest.open('POST', 'tabulate-json.php', true);
        httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

        httpRequest.onreadystatechange = function() { updateRows(httpRequest); } ;
        httpRequest.send(data);
        t = setTimeout("refreshPage()", 60000);
    }
</script>
</head>
<body onload = "refreshPage()">
<center>
<script type="text/javascript">
   alert("Now loading the survey");
</script>
<form name = "pollForm">
<table id = "theTable" border = "1" class="thetable">
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
    var theCDs = new Array(), CDcount = 0, t;
</script>
</body>
</html>
