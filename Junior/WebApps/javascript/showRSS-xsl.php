<html>
<head>
<title>EXTREMELY PRIMITIVE RSS Reader</title>
<script type="text/javascript">
    // CS 1520 Summer 2012
    // Compare this to showRSS.php
    
    // In this case, AJAX is used to read in an XSL stylesheet, then to
    // read in the XML file.  It then applies the stylesheet tranformations
    // to the file and adds the result to the document tree.

    // One advantage to this approach is that the various HTML tags that are
    // being used to generate the table can be coded in a normal way in the
    // XSL stylesheet [rather than the wordy DOM approach]

    // See also formatRSS.xsl

    // Read in the stylesheet from the server
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
	  xhttp.open("GET","formatRSS.xsl",true);
	  xhttp.onreadystatechange = function() { returnXSL(xhttp); };
	  xhttp.send("");
    }

    // Once the stylesheet has been read in, read in and parse the
    // RSS document
    function returnXSL(xhttp)
    {
          if (xhttp.readyState == 4)
          {
             if (xhttp.status == 200)
             {
                xsl = xhttp.responseXML;
			    processData();
             }
          }
    }

    function processData() {
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
 
        httpRequest.open('GET', 'http://cs1520.cs.pitt.edu/~nomad/xml/rss2sample.xml', true);
        httpRequest.setRequestHeader('Content-Type', 'text/xml');
        httpRequest.setRequestHeader('Cache-Control', 'no-cache');

        httpRequest.onreadystatechange = function() { showFeed(httpRequest); };
        httpRequest.send(null);
    }

    function showFeed(httpRequest)
    {
        if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
		// Note: This will not work IE up through IE 8.  Additional
		// code would be required to get it to work there
            var root = httpRequest.responseXML;
	        var xsltProcessor=new XSLTProcessor();
	        xsltProcessor.importStylesheet(xsl);
	        var resultDocument = xsltProcessor.transformToFragment(root,document);
	        document.body.appendChild(resultDocument);
	   }
	   else
	   {   alert('Problem with request'); }
        }
    }
</script>
</head>
<body>
	<script type="text/javascript">
  	   var xsl;
	   getXSL();
	</script>
</body>
</html>
