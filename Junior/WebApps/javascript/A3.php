<!DOCTYPE html>
<html>
<head>
<title>Assignment 3</title>

<script type="text/javascript">
// CS 1520 Summer 2013 -- Compare to CDpoll2.php
// This has the same functionality as CDpoll2.php, with the addition of 
// an automatic update of the table rows if any have been added to the DB.
// A timer calls an update method once every minute, and if any new rows
// have been added to the DB, they are passed on to this script and added
// to the table.  Note that we still do not update counts from the server
// automatically -- they only change if we select a given CD.  Also, we now
// have to consider data consistency on the client side -- see more comments
// about this below.

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

        httpRequest.open('POST', 'tabulate-update.php', true);
        httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

        if (type == 1)
              httpRequest.onreadystatechange = function() { showResults(httpRequest, choice); };
        else
              httpRequest.onreadystatechange = function() { addNewRows(httpRequest, title, artist); } ;
        httpRequest.send(data);
    }

    function showResults(httpRequest, choice)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
               var newCount = httpRequest.responseText;

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

    // Add 1 or more rows to the table (based on the responseText
    // string).  
    function addNewRows(httpRequest, title, artist)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
               var ack = httpRequest.responseText;
	           //alert(ack);
               if (ack == "OK")
               {
		   		   addRow(title, artist, '1');
               }
               else
	       {
	           var newRows = ack.split("^");
	           for (var i = 0; i < newRows.length; i++)
	           {
		           var theRow = newRows[i].split("|");
		           addRow(theRow[1], theRow[2], theRow[3]);
	           }
		           addRow(title, artist, '1');
	       }
           }
           else
           {   alert('Problem with request'); }
           // Process has completed so reset the pending variable to allow
           // another update to occur
           pending = false;
       }
    }

    // This is similar to the addNewRows function above, but does
    // not add any local rows -- only rows from the server are added
    // if they exist.
    function updateRows(httpRequest)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
               var ack = httpRequest.responseText;
               if (ack != "")
               {
                  var newRows = ack.split("^");
	          for (var i = 0; i < newRows.length; i++)
	          {
	              var theRow = newRows[i].split("|");
		      addRow(theRow[1], theRow[2], theRow[3]);
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
          // Process has completed so reset the pending variable to allow another
          // update to occur
          pending = false;
       }
    }

    function addRow(title, artist, vote)
    {
          var T = document.getElementById("theTable");
          var len = T.rows.length;
          var R = T.insertRow(len); 
          R.align = 'center';       

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
          C.appendChild(rb);
    }

    // This function is called when the page is loaded.  It sets a timer
    // to call the refreshPage function after 1 minute.
    function startRefresh()
    {
	     t = setTimeout("refreshPage()", 60000);
    } 

    // Make an AJAX call to see if there are any new rows in the DB. 
    // If so, the callback method adds them to the table.  Note that the
    // callback method sets the timer again (and this repeats
    // indefinitely).
    // Note: Now our logic is complicated just a bit.  It is possible that
    // a user could enter a new CD just as the system is automatically updating
    // the list, but before the update has completed.  In this case we could have
    // a consistency problem:  Both requests have gone to the server but neither
    // has returned, so upon return the table is updated twice with the new CD
    // entries.  To avoid this problem, we can keep a state variable.  The
    // pending variable will only allow one active request to update the page
    // at a time.  When the request completes the variable is reset to allow
    // another update.  In the case of the autoupdate below, if there is a pending
    // request we simply don't do the update request and reset the timer for the
    // next request.  However, in the case of the user entered information, we
    // need to tell the user to try again -- see processWritein
    function refreshPage()
    {
        if (!pending)
        {
            pending = true;
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

            httpRequest.open('POST', 'tabulate-update.php', true);
            httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');

            httpRequest.onreadystatechange = function() { updateRows(httpRequest); } ;
            httpRequest.send(data);   
        }
        t = setTimeout("refreshPage()", 60000);
    }
</script>

	