<!--main for Assignment 3 1520 Andrew Beers 7/15/2013 -->
<?php
	session_start();
	if ( !isset($_SESSION['option'] ) )
	{
		header("Location: Assignment3.php");
	}
	
	function __autoload( $class )
	{

		require_once $class . '.php';
	}
	include 'A2Functions.php';
	require_once 'A2User.php';
?>
<html>
<head>
<link rel = "stylesheet" type = "text/css" href = "A3Style.css"/>
<title>Assignment 3</title>
<script type="text/javascript">
	
	var studentCourseArray = new Array();
	var numCourses = 0;
	function Logout()
	{
		
		alert("Logging off");
		location.reload(true)
	
	}
	//change font
	 function showSelected()
    {
          var rowind = arguments[0];
          var table = arguments[1];
         
          var theTable = document.getElementById(table);
	  	  var theRow = theTable.rows[rowind];
          theRow.className = "highlight";
    }

    function makeRegular()
    {
          var rowind = arguments[0];
          var table = arguments[1];
          
          var theTable = document.getElementById(table);
	  	  var theRow = theTable.rows[rowind];
          theRow.className = "regular";
    }
	
	//create menu box
	function createBackToLogin( body, psid, access )
	{
		
		form = document.createElement("form");
		form.setAttribute('id','backToLogin');
		
		var tableTAG2 = document.createElement("table");
		tableTAG2.setAttribute('id','table2');
		tableTAG2.border = 1;
		tableTAG2.align = 'center';
		body.appendChild(tableTAG2);
		var hrow = tableTAG2.insertRow(0);
		hrow.className = "regular";
		hrow.align = 'center';
		var C = hrow.insertCell(0);
		var cellContents = document.createTextNode('Back to Menu');
		C.appendChild(cellContents);
		
		C = hrow.insertCell(1);
		input = document.createElement("input");
		input.setAttribute('type','radio');
		//alert(psid);
		//alert(access);
		input.setAttribute('onclick',"Login('" + psid + "','" +access + "')");
		input.setAttribute('onmouseover',"showSelected('0','table2')");
		input.setAttribute('onmouseout',"makeRegular('0','table2')");
	
		C.appendChild(input);
		
		var hrow = tableTAG2.insertRow(1);
		hrow.align = 'center';
		var C = hrow.insertCell(0);
		var cellContents = document.createTextNode('Logout');
		hrow.className = "regular";
		C.appendChild(cellContents);
		
		
		C = hrow.insertCell(1);
		input = document.createElement("input");
		input.setAttribute('type','radio');
		input.setAttribute('onclick',"Logout()");
		input.setAttribute('onmouseover',"showSelected('1','table2')");
		input.setAttribute('onmouseout',"makeRegular('1','table2')");
		
		C.appendChild(input);
	
		
		form.appendChild(tableTAG2);
		
		return form;
	}
	
	//handle advisor actions
	function advisorAction(action, psid, access )
	{
	
		var httpRequest;
		if (window.XMLHttpRequest) 
		{ // Mozilla, Safari, ...
			httpRequest = new XMLHttpRequest();
			if (httpRequest.overrideMimeType) 
			{
				httpRequest.overrideMimeType('text/xml');
			}
		}
		else if (window.ActiveXObject) 
		{ // IE
			try 
			{
				httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch (e) 
			{
				try 
				{
					httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
				}
				catch (e) {}
			}
		}
		if (!httpRequest) 
		{
			alert('Giving up :( Cannot create an XMLHTTP instance');
			return false;
		}

		var data;
	
		data = 'action=' + action + '&' + 'psid=' + psid;
		

		httpRequest.open('POST', 'advisorProcessing.php',true);
		httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		//httpRequest.setRequestHeader('Content-Type', 'text/xml');

		httpRequest.onreadystatechange = function() { showAdvisorResults(httpRequest, action, psid, access); };

		httpRequest.send(data);
		
	}

	//show advisor results after an action
	function showAdvisorResults(httpRequest, action, psid, access)
	{
		if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
           		if ( action == 1 )
           		{
           			/*
           			//alert(access);
					var bodyTAG = document.getElementsByTagName("body")[0];
					var formTAG = document.getElementsByTagName("form")[0];
					var oldTable = document.getElementById('theTable');
				
					if ( oldTable != null )
					{
						bodyTAG.removeChild(oldTable);
						bodyTAG.removeChild(formTAG);
					
					}
					*/
					var ack = httpRequest.responseText;
					
					if ( ack != null )
					{
						alert(ack);
					}
					else
					{
						alert("An Error Occurred.");
					}
           			
           		
           		}
           		else if ( action == 2 || action == 3 || action == 4)
           		{
           		
           			var ack = httpRequest.responseText;
           			
           			alert( ack );
           			if ( ack != null )
           			{	
           				var xml = httpRequest.responseXML;
           				var root = httpRequest.responseXML.documentElement;
               			var rootattr = root.attributes;
               			var sessions = rootattr.getNamedItem("sessions").value;
           				
					
						var bodyTAG = document.getElementsByTagName("body")[0];
						var formTAG1 = document.getElementsByName('pollForm')[0];
						var formTAG2 = document.getElementById("backToLogin");
						var formTAG3 = document.getElementById("advisorOptions");
						var header = document.getElementsByTagName("h3")[0];
				
				
						bodyTAG.removeChild(formTAG1);
						bodyTAG.removeChild(formTAG2);
						bodyTAG.removeChild(formTAG3);
						bodyTAG.removeChild(header);
					
						
						
						header = document.createElement('h1');
						header.align = 'center';
			
						text = document.createTextNode('Session List');
			
						header.appendChild(text);
						bodyTAG.appendChild(header);
						
						var tableTAG2 = document.createElement("table");
						tableTAG2.setAttribute('id','table2');
						tableTAG2.border = 1;
						tableTAG2.align = 'center';
						bodyTAG.appendChild(tableTAG2);
						var hrow = tableTAG2.insertRow(0);
						hrow.align = 'center';
						var C = hrow.insertCell(0);
						var cellContents = document.createTextNode('Date');
						C.appendChild(cellContents);
						
						var C = hrow.insertCell(1);
						var cellContents = document.createTextNode('PeopleSoftID');
						C.appendChild(cellContents);
						
						if ( action > 2 )
						{
							var C = hrow.insertCell(2);
							var cellContents = document.createTextNode('Select');
							C.appendChild(cellContents);
						}

						
						for (var i = 0; i < sessions; i++)
						{
						
							hrow = tableTAG2.insertRow(i+1);
							hrow.align = 'center';
							
							date = root.getElementsByTagName('date')[i].childNodes[0].nodeValue;
							contents = date;
							
							if ( contents == null )
							{
								contents = "";
							}
							
							C = hrow.insertCell(0);
							cellContents = document.createTextNode(contents);
							C.appendChild(cellContents);
							
							contents = root.getElementsByTagName('psid')[i].childNodes[0];
							if ( contents != null )
							{
								contents = contents.nodeValue;
							}
							else
							{
								contents = "";
							}
							
							C = hrow.insertCell(1);
							cellContents = document.createTextNode(contents);
							C.appendChild(cellContents);
							
							if ( action > 2 )
							{
							
								C = hrow.insertCell(2);
								input = document.createElement("input");
								input.setAttribute('type','radio');
								
								input.setAttribute('onclick',"sessionAction('"+ date + "','" + psid + "','" + action + "','" + i + "')");
	
								C.appendChild(input);
							
							
							}
						
						}
						
					
						form = createBackToLogin(bodyTAG,psid,access);
						
						bodyTAG.appendChild(form);
				
					}
           		}
           }
        }
	}
	
	
	//add or enter notes
	function sessionAction(date, psid, action, i )
	{
		
		if ( action == 3 )
		{
			var body = document.getElementsByTagName("body")[0];
			var table = document.getElementById("table2");
			var formTAG2 = document.getElementById("backToLogin");
			
			if ( table != null )
			{
				body.removeChild(table);
				body.removeChild(formTAG2);
			}
			
			var tableTAG2 = document.createElement("table");
			tableTAG2.setAttribute('id','table2');
			tableTAG2.border = 1;
			tableTAG2.align = 'center';
			body.appendChild(tableTAG2);
			var hrow = tableTAG2.insertRow(0);
			hrow.align = 'center';
			var C = hrow.insertCell(0);
			var cellContents = document.createTextNode('Enter Note:');
			C.appendChild(cellContents);

			
			C = hrow.insertCell(1);
			
			input = document.createElement("input");
			input.setAttribute('type','text');
			input.setAttribute('name','note');
			input.setAttribute('value', "");
			C.appendChild(input);
			
			var hrow = tableTAG2.insertRow(1);
			hrow.align = 'center';
			var C = hrow.insertCell(0);
			var cellContents = document.createTextNode('Submit');
			C.appendChild(cellContents);

			
			C = hrow.insertCell(1);
			input = document.createElement("input");
			input.setAttribute('type','radio');
		
			input.setAttribute('onclick',"displaySession('"+ date + "','" + psid + "','" + action + "','" + i + "')");

			C.appendChild(input);
			
			
			form = createBackToLogin(body, psid, 1);
			body.appendChild(form);

		
			
		}
		else if ( action == 4 )
		{
		
			
			if (window.XMLHttpRequest) 
			{ // Mozilla, Safari, ...
				httpRequest = new XMLHttpRequest();
				if (httpRequest.overrideMimeType) 
				{
					httpRequest.overrideMimeType('text/xml');
				}
			}
			else if (window.ActiveXObject) 
			{ // IE
				try 
				{
					httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
				}
				catch (e) 
				{
					try 
					{
						httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
					}
					catch (e) {}
				}
			}
			if (!httpRequest) 
			{
				alert('Giving up :( Cannot create an XMLHTTP instance');
				return false;
			}

			var data;
	
			data = 'date=' + date + '&' + 'psid=' + psid+ '&' + 'action=' + action;
	

			httpRequest.open('POST', 'advisorProcessing.php',true);
			httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
			//httpRequest.setRequestHeader('Content-Type', 'text/xml');

			httpRequest.onreadystatechange = function() { addNotes(httpRequest, action, psid); };

			httpRequest.send(data);
		
	
			}
	
	}
	
	//display logged sessions
	function displaySession( date, psid, action, i )
	{
	
		var httpRequest;
		note = document.getElementsByName('note')[0].value;
		
		if ( note == null )
		{
			alert("Note empty. Note must contain text.");
			
		}
		
		
		if (window.XMLHttpRequest) 
		{ // Mozilla, Safari, ...
			httpRequest = new XMLHttpRequest();
			if (httpRequest.overrideMimeType) 
			{
				httpRequest.overrideMimeType('text/xml');
			}
		}
		else if (window.ActiveXObject) 
		{ // IE
			try 
			{
				httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch (e) 
			{
				try 
				{
					httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
				}
				catch (e) {}
			}
		}
		if (!httpRequest) 
		{
			alert('Giving up :( Cannot create an XMLHTTP instance');
			return false;
		}

		var data;
		
		data = 'date=' + date + '&' + 'psid=' + psid+ '&' + 'action=' + action + '&' + 'note=' + note;
		

		httpRequest.open('POST', 'advisorProcessing.php',true);
		httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		//httpRequest.setRequestHeader('Content-Type', 'text/xml');

		httpRequest.onreadystatechange = function() { addNotes(httpRequest, action, psid); };

		httpRequest.send(data);
	
	
	
	}
	
	
	function addNotes( httpRequest, action, psid )
	{
		if (httpRequest.readyState == 4)
		{
		   if (httpRequest.status == 200)
		   {
				if ( action == 3 )
				{
					var note = httpRequest.responseText;
					
					alert( note );
		
				}
				else if ( action == 4 )
				{
		
					var note = httpRequest.responseText;
					
					alert( note );
		
				}
			}
			
		}
	
	
	}
	//sorting functions
	function by_term(a, b )
	{
		if ( a.term < b.term )
		{
			result = -1;
		}
		else if ( a.term == b.term)
		{
			result = 0;
		}
		else
		{
			result = 1;
		}
		return result;
	
	}
	
	function by_num(a, b )
	{
		if ( a.classnum < b.classnum )
		{
			result = 1;
		}
		else if ( a.classnum == b.classnum)
		{
			result = 0;
		}
		else
		{
			result = -1;
		}
		return result;
	
	}
	
	function by_name(a, b )
	{
		if ( a.dept < b.dept )
		{
			result = -1;
		}
		else if ( a.dept == b.dept)
		{
			result = -1;
		}
		else
		{
			result = 1;
		}
		return result;
	
	}
	
	//main data handler
	function processData(type, setting, psid, access)
	{
	
		if ( type == 1 )
		{
			if ( setting == 3 )
			{
			
				var oldTable = document.getElementById('theTable');
				var formTAG2 = document.getElementById("backToLogin");
				var body = document.getElementsByTagName("body")[0];
				var poll = body.getElementsByTagName("form")[0];
				if ( oldTable != null )
				{
					poll.removeChild(oldTable);
					body.removeChild(formTAG2);
				}
				var httpRequest;
		
				if (window.XMLHttpRequest) 
				{ // Mozilla, Safari, ...
					httpRequest = new XMLHttpRequest();
					if (httpRequest.overrideMimeType) 
					{
						httpRequest.overrideMimeType('text/xml');
					}
				}
				else if (window.ActiveXObject) 
				{ // IE
					try 
					{
						httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
					}
					catch (e) 
					{
						try 
						{
							httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
						}
						catch (e) {}
					}
				}
				if (!httpRequest) 
				{
					alert('Giving up :( Cannot create an XMLHTTP instance');
					return false;
				}
		
				var data = 'type=' + type + '&' + 'setting=' + setting + '&' + 'psid=' + psid;
				httpRequest.open('POST', 'choiceProcessing.php',true);
				httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
				//httpRequest.setRequestHeader('Content-Type', 'text/xml');
		
				httpRequest.onreadystatechange = function() { showResults(httpRequest, setting, access, psid); };
		
				httpRequest.send(data);
			}
			else
			{
				var oldTable = document.getElementById('theTable');
				var formTAG2 = document.getElementById("backToLogin");
				var body = document.getElementsByTagName("body")[0];
				var poll = body.getElementsByTagName("form")[0];
				if ( oldTable != null )
				{
					poll.removeChild(oldTable);
					body.removeChild(formTAG2);
				}
			
				var header = document.createElement('h1');
				header.align = 'center';
			
			
				if ( setting == 1 )
				{
					studentCourseArray.sort(by_term);
					
				
					text = document.createTextNode('Term-Term Class List');
						
					
				}
				else if ( setting == 2 )
				{
					studentCourseArray.sort(by_num);
					studentCourseArray.sort(by_name);
				
					text = document.createTextNode('Alphabetical Class List');

				}
				header.appendChild(text);
				body.appendChild(header);
				var theTable = document.createElement('table');
				theTable.setAttribute('id','theTable');
				theTable.border = 1;
				theTable.align = 'center';
				body.appendChild(theTable);
				var hrow = theTable.insertRow(0);
				hrow.align = 'center';
				var C = hrow.insertCell(0);
				var cellContents = document.createTextNode('Term');
				C.appendChild(cellContents);
			
				C = hrow.insertCell(1);
				cellContents = document.createTextNode('Department');
				C.appendChild(cellContents);
			
				C = hrow.insertCell(2);
				cellContents = document.createTextNode('Class Number');
				C.appendChild(cellContents);
			
				C = hrow.insertCell(3);
				cellContents = document.createTextNode('Grade');
				C.appendChild(cellContents);
			
				for (var i = 0; i < numCourses; i++)
				{
			
					hrow = theTable.insertRow(i+1);
					hrow.align = 'center';
				
					contents = studentCourseArray[i].term;
					C = hrow.insertCell(0);
					cellContents = document.createTextNode(contents);
					C.appendChild(cellContents);
				
					contents = studentCourseArray[i].dept;
								
					C = hrow.insertCell(1);
					cellContents = document.createTextNode(contents);
					C.appendChild(cellContents);
			
					contents = studentCourseArray[i].classnum;
					C = hrow.insertCell(2);
					cellContents = document.createTextNode(contents);
					C.appendChild(cellContents);
			
					contents = studentCourseArray[i].grade;				
					C = hrow.insertCell(3);
					cellContents = document.createTextNode(contents);
					C.appendChild(cellContents);
				
				
				}
				form = createBackToLogin( body , psid, access );
					
				body.appendChild(form);
			}
			
			
		}
		else if ( type == 2 )
		{
			if ( setting == 1 )
			{
				var oldTable = document.getElementById('theTable');
				var body = document.getElementsByTagName("body")[0];
				var poll = body.getElementsByTagName("form")[0];
				var pollForm = document.getElementById('backToLogin');
				
				if ( oldTable != null )
				{
					poll.removeChild(oldTable);
					body.removeChild(poll);
					body.removeChild(pollForm);
				
				}
			
				var header = document.createElement('h1');
				header.align = 'center';

				text = document.createTextNode('Search Student');
			
				header.appendChild(text);
			
				body.appendChild(header);
				header = document.createElement('h3');
				header.align = 'center';
			
				text = document.createTextNode('Enter First and Last name or PeopleSoftID');
			
				header.appendChild(text);
			
				body.appendChild(header);
			
			
				var theTable = document.createElement('table');
				theTable.setAttribute('id','theTable');
				theTable.border = 1;
				theTable.align = 'center';
				body.appendChild(theTable);
				var hrow = theTable.insertRow(0);
				hrow.align = 'center';
				var C = hrow.insertCell(0);
				var cellContents = document.createTextNode('First Name');
				C.appendChild(cellContents);
			
				C = hrow.insertCell(1);
			
				input = document.createElement("input");
				input.setAttribute('type','text');
				input.setAttribute('name','first');
				input.setAttribute('value', "");
				C.appendChild(input);
			
				var hrow = theTable.insertRow(1);
				hrow.align = 'center';
			
				var C = hrow.insertCell(0);
				var cellContents = document.createTextNode('Last Name');
				C.appendChild(cellContents);
			
				C = hrow.insertCell(1);
			
				input = document.createElement("input");
				input.setAttribute('type','text');
				input.setAttribute('name','last');
				input.setAttribute('value', "");
				C.appendChild(input);
			
				var hrow = theTable.insertRow(2);
				hrow.align = 'center';
			
				var C = hrow.insertCell(0);
				var cellContents = document.createTextNode('PeopleSoft ID');
				C.appendChild(cellContents);
			
				C = hrow.insertCell(1);
			
				input = document.createElement("input");
				input.setAttribute('type','text');
				input.setAttribute('name','psid');
				input.setAttribute('value', "");
				C.appendChild(input);
			
			
				var hrow = theTable.insertRow(3);
				hrow.align = 'center';
			
				var C = hrow.insertCell(0);
				var cellContents = document.createTextNode('Submit');
				C.appendChild(cellContents);
			
				C = hrow.insertCell(1);
			
				input = document.createElement("input");
				input.setAttribute('type','radio');
				
				input.setAttribute('id', 'submitted');
				input.setAttribute('onclick',"processData('2', '2','" + psid + "','" + access +"')");
				
				C.appendChild(input);
				
				
				form = createBackToLogin(body, psid, access);
				body.appendChild(form);
				
			}
			else if ( setting == 2 )
			{
				
				var invalid = 1;
				first = document.getElementsByName('first')[0].value;
				last = document.getElementsByName('last')[0].value;
				psid = document.getElementsByName('psid')[0].value;
				
				if ( psid != null )
				{

					var matches1 = psid.match(/[0-9]{7}/);
					
					if ( matches1 )
					{
						getCourses(0 , psid );
						invalid = 0;
						Login(psid, 0 );
						var body = document.getElementsByTagName("body")[0];
               				header = document.createElement("h3");
               				header.align = 'center';
               				text = document.createTextNode("Advisor Options");
               				
               				header.appendChild(text);
               				
               				body.appendChild(header);
               				
               				form = document.createElement("form");
						  	form.setAttribute('id','advisorOptions');
						  	
						  	var tableTAG2 = document.createElement("table");
						  	tableTAG2.setAttribute('id','table2');
               				tableTAG2.border = 1;
               				tableTAG2.align = 'center';
               				body.appendChild(tableTAG2);
               				var hrow = tableTAG2.insertRow(0);
               				hrow.align = 'center';
						  	var C = hrow.insertCell(0);
               				var cellContents = document.createTextNode('Add Session');
               				C.appendChild(cellContents);
						  	
						  	
						  	C = hrow.insertCell(1);
						  	input = document.createElement("input");
						  	input.setAttribute('type','radio');
						  	//alert(psid);
						  	//alert(access);
						  	input.setAttribute('onclick',"advisorAction('1','" + psid + "','" +access + "')");
						  	C.appendChild(input);
						  	
						  	var hrow = tableTAG2.insertRow(1);
						  	hrow.align = 'center';
               				var C = hrow.insertCell(0);
               				var cellContents = document.createTextNode('Review Sessions');
               				C.appendChild(cellContents);
						  	
						  	
						  	C = hrow.insertCell(1);
						  	input = document.createElement("input");
						  	input.setAttribute('type','radio');
						  	input.setAttribute('onclick',"advisorAction('2','" + psid + "','" +access + "')");
						  	
						  	C.appendChild(input);
						  	
						  	
						  	var hrow = tableTAG2.insertRow(2);
               				hrow.align = 'center';
						  	var C = hrow.insertCell(0);
               				var cellContents = document.createTextNode('Add Notes');
               				C.appendChild(cellContents);
						  	
						  	
						  	C = hrow.insertCell(1);
						  	input = document.createElement("input");
						  	input.setAttribute('type','radio');
						  	//alert(psid);
						  	//alert(access);
						  	input.setAttribute('onclick',"advisorAction('3','" + psid + "','" +access + "')");
						  	C.appendChild(input);
						  	
						  	var hrow = tableTAG2.insertRow(3);
						  	hrow.align = 'center';
               				var C = hrow.insertCell(0);
               				var cellContents = document.createTextNode('Review Notes');
               				C.appendChild(cellContents);
						  	
						  	
						  	C = hrow.insertCell(1);
						  	input = document.createElement("input");
						  	input.setAttribute('type','radio');
						  	input.setAttribute('onclick',"advisorAction('4','" + psid + "','" +access + "')");
						  	
						  	C.appendChild(input);
						
						  	
						  	form.appendChild(tableTAG2);
						  	
						  	body.appendChild(form);
					
					}
					else
					{
						alert("Invalid PeopleSoftID. Please enter 7 digits");
						
						document.getElementsByName('first')[0].value = "";
						document.getElementsByName('last')[0].value = "";
						document.getElementsByName('psid')[0].value = "";
						
					}
				
					

					
				}
				else if ( first != null && last != null )
				{
				
					invalid = 0;
					var httpRequest;
				
				
		
					if (window.XMLHttpRequest) 
					{ // Mozilla, Safari, ...
						httpRequest = new XMLHttpRequest();
						if (httpRequest.overrideMimeType) 
						{
							httpRequest.overrideMimeType('text/xml');
						}
					}
					else if (window.ActiveXObject) 
					{ // IE
						try 
						{
							httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
						}
						catch (e) 
						{
							try 
							{
								httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
							}
							catch (e) {}
						}
					}
					if (!httpRequest) 
					{
						alert('Giving up :( Cannot create an XMLHTTP instance');
						return false;
					}
			
					var data;
					
					data = 'type=' + type + '&' + 'setting=' + setting + '&' + 'first=' + first + '&' + 'last=' + last;
					
			
					httpRequest.open('POST', 'choiceProcessing.php',true);
					httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
					//httpRequest.setRequestHeader('Content-Type', 'text/xml');
	
					httpRequest.onreadystatechange = function() { showResults(httpRequest, setting, access, psid, type); };
	
					httpRequest.send(data);
				
					
					
				}
				else
				{
					alert("error");
				
				}
				
				
			
			
			}
		
		
		}
		else if ( type == 3 )
		{
			
			if ( setting == 1 )
			{
			
				var oldTable = document.getElementById('theTable');
				var body = document.getElementsByTagName("body")[0];
				var poll = document.getElementsByTagName("form")[0];
				
				var pollForm = document.getElementById('backToLogin');
				if ( oldTable != null )
				{
					poll.removeChild(oldTable);
					body.removeChild(poll);
					body.removeChild(pollForm);
			
				}
		
				var header = document.createElement('h1');
				header.align = 'center';

				text = document.createTextNode('Search Course');
		
				header.appendChild(text);
					
				body.appendChild(header);
		
		
				var theTable = document.createElement('table');
				theTable.setAttribute('id','theTable');
				theTable.border = 1;
				theTable.align = 'center';
				body.appendChild(theTable);
				var hrow = theTable.insertRow(0);
				hrow.align = 'center';
				var C = hrow.insertCell(0);
				var cellContents = document.createTextNode('Department');
				C.appendChild(cellContents);
		
				C = hrow.insertCell(1);
		
				input = document.createElement("input");
				input.setAttribute('type','text');
				input.setAttribute('name','department');
				input.setAttribute('value', "");
				C.appendChild(input);
		
				var hrow = theTable.insertRow(1);
				hrow.align = 'center';
		
				var C = hrow.insertCell(0);
				var cellContents = document.createTextNode('Course Number');
				C.appendChild(cellContents);
		
				C = hrow.insertCell(1);
		
				input = document.createElement("input");
				input.setAttribute('type','text');
				input.setAttribute('name','course');
				input.setAttribute('value', "");
				C.appendChild(input);
		
				var hrow = theTable.insertRow(2);
				hrow.align = 'center';
		
				var C = hrow.insertCell(0);
				var cellContents = document.createTextNode('Submit');
				C.appendChild(cellContents);
		
				C = hrow.insertCell(1);
		
				input = document.createElement("input");
				input.setAttribute('type','radio');
				input.setAttribute('id', 'submitted');
				input.setAttribute('onclick',"processData('3', '2','" + psid + "','" + access +"')");
			
				C.appendChild(input);
				
				form = createBackToLogin( body, psid, access );
				
				body.appendChild(form);
			
			
			}
			else if ( setting == 2 )
			{
				var invalid = 1;
				course = document.getElementsByName('course')[0].value;
				department = document.getElementsByName('department')[0].value;
				
				
				if ( course != null && department != null )
				{
					invalid = 0;
					var httpRequest;
				
				
		
					if (window.XMLHttpRequest) 
					{ // Mozilla, Safari, ...
						httpRequest = new XMLHttpRequest();
						if (httpRequest.overrideMimeType) 
						{
							httpRequest.overrideMimeType('text/xml');
						}
					}
					else if (window.ActiveXObject) 
					{ // IE
						try 
						{
							httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
						}
						catch (e) 
						{
							try 
							{
								httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
							}
							catch (e) {}
						}
					}
					if (!httpRequest) 
					{
						alert('Giving up :( Cannot create an XMLHTTP instance');
						return false;
					}
			
					var data;
					
					data = 'type=' + type + '&' + 'setting=' + setting + '&' + 'course=' + course + '&' + 'department=' + department;
					
			
					httpRequest.open('POST', 'choiceProcessing.php',true);
					httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
					//httpRequest.setRequestHeader('Content-Type', 'text/xml');
	
					httpRequest.onreadystatechange = function() { showResults(httpRequest, setting, access, psid, type); };
	
					httpRequest.send(data);
			
			}
			
		}
		}
	
	
	}
	
	function showResults(httpRequest, setting, access, psid, type)
	{
		if (httpRequest.readyState == 4)
        {
           if (httpRequest.status == 200)
           {
           		
               if ( access == 0 )
               {
               		
               		if (setting == 1 || setting == 2)
               		{
               			var ack = httpRequest.responseText;
               			
               			
               			//alert(ack);
               			if (ack != "")
					    {
						   	var xml = httpRequest.responseXML;
               				var root = httpRequest.responseXML.documentElement;
               				var rootattr = root.attributes;
               				var courses = rootattr.getNamedItem("courses").value;
               				//alert(courses)
               				
               				var oldTable = document.getElementById('theTable');
               				var body = document.getElementsByTagName("body")[0];
               				var poll = body.getElementsByTagName("form")[0];
               				if ( oldTable != null )
               				{
               					poll.removeChild(oldTable);
               					
               				}
               				
               				var header = document.createElement('h1');
               				header.align = 'center';
               				
               				if ( setting == 1 )
               				{
               					text = document.createTextNode('Term-Term Class List');
               					
               				}
               				else
               				{
               					text = document.createTextNode('Alphabetical Class List');
               				
               				}
               				
               				header.appendChild(text);
               				body.appendChild(header);
               				var theTable = document.createElement('table');
               				theTable.setAttribute('id','theTable');
               				theTable.border = 1;
               				theTable.align = 'center';
               				body.appendChild(theTable);
               				var hrow = theTable.insertRow(0);
               				hrow.align = 'center';
               				var C = hrow.insertCell(0);
               				var cellContents = document.createTextNode('Term');
               				C.appendChild(cellContents);
               				
               				C = hrow.insertCell(1);
               				cellContents = document.createTextNode('Department');
               				C.appendChild(cellContents);
               				
               				C = hrow.insertCell(2);
               				cellContents = document.createTextNode('Class Number');
               				C.appendChild(cellContents);
               				
               				C = hrow.insertCell(3);
               				cellContents = document.createTextNode('Grade');
               				C.appendChild(cellContents);
               				
               				
						   	for (var i = 0; i < courses; i++)
						   	{
						   	
						   		hrow = theTable.insertRow(i+1);
               					hrow.align = 'center';
               					
               					contents = root.getElementsByTagName('Term')[i].childNodes[0];
               					if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
               					C = hrow.insertCell(0);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
               					
               					contents = root.getElementsByTagName('Department')[i].childNodes[0];
               					if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
               					C = hrow.insertCell(1);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
							
								contents = root.getElementsByTagName('ClassNumber')[i].childNodes[0];
								if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
								C = hrow.insertCell(2);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
							
								contents = root.getElementsByTagName('Grade')[i].childNodes[0];
								if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
								C = hrow.insertCell(3);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
							   	
						  	   	
						  	}
						  	
						  	form = createBackToLogin( body , psid, access );
						  	
						  	body.appendChild(form);
							  	
							window.status="Table updated at " + (new Date()).toString();
						}
					 	
               		}
               		else if ( setting == 3 )
               		{
               			var ack = httpRequest.responseText;
               			
               			
               			//alert(ack);
               			if (ack != "")
					    {
						   	var xml = httpRequest.responseXML;
               				var root = httpRequest.responseXML.documentElement;
               				
               				var rootattr = root.attributes;
               				var courses = rootattr.getNamedItem("courses").value;
               				//alert(courses)
               				
               				var oldTable = document.getElementById('theTable');
               				var body = document.getElementsByTagName("body")[0];
               				var poll = body.getElementsByTagName("form")[0];
               				if ( oldTable != null )
               				{
               					poll.removeChild(oldTable);
               					
               				}
               				
               				var header = document.createElement('h1');
               				header.align = 'center';
               				
               				text = document.createTextNode('Progress Report');
               				
               				header.appendChild(text);
               				body.appendChild(header);
               				var theTable = document.createElement('table');
               				theTable.setAttribute('id','theTable');
               				theTable.border = 1;
               				theTable.align = 'center';
               				body.appendChild(theTable);
               				var hrow = theTable.insertRow(0);
               				hrow.align = 'center';
               				var C = hrow.insertCell(0);
               				var cellContents = document.createTextNode('Satisfied');
               				C.appendChild(cellContents);
               				
               				C = hrow.insertCell(1);
               				cellContents = document.createTextNode('Course');
               				C.appendChild(cellContents);
               				
               				C = hrow.insertCell(2);
               				cellContents = document.createTextNode('Term');
               				C.appendChild(cellContents);
               				
               				C = hrow.insertCell(3);
               				cellContents = document.createTextNode('Grade');
               				C.appendChild(cellContents);
               				
               				C = hrow.insertCell(4);
               				cellContents = document.createTextNode('GPA');
               				C.appendChild(cellContents);
               				
               				
						   	for (var i = 0; i < courses; i++)
						   	{
						   	
						   		hrow = theTable.insertRow(i+1);
               					hrow.align = 'center';
               					
               					contents = root.getElementsByTagName('requirement')[i].childNodes[0];
               					if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
               					C = hrow.insertCell(0);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
               					
               					contents = root.getElementsByTagName('class')[i].childNodes[0];
               					if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
               					C = hrow.insertCell(1);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
							
								contents = root.getElementsByTagName('term')[i].childNodes[0];
								if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
								C = hrow.insertCell(2);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
							
								contents = root.getElementsByTagName('grade')[i].childNodes[0];
								if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
								C = hrow.insertCell(3);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
							   	
							   	
							   	contents = root.getElementsByTagName('gpa')[i].childNodes[0];
								if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
								C = hrow.insertCell(4);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
						  	   	
						  	}
						  	
						  	form = createBackToLogin( body, psid, access);
						  	
						  	body.appendChild(form);
               				
               			}
               		
               		}
               }
               else if ( access == 1 || access == 2)
               {
               		if ( type == 1 )
               		{
               		
               		}
               		else if ( type == 2 )
               		{
               		
						if (setting == 1 )
						{
						//search student
						}
						else if (setting == 2 )
						{
					
							var ack = httpRequest.responseText;
						
							if (ack != "")
							{
								alert(ack);
								Login( ack, 0 );
							
								var body = document.getElementsByTagName("body")[0];
								header = document.createElement("h3");
								header.align = 'center';
								text = document.createTextNode("Advisor Options");
							
								header.appendChild(text);
							
								body.appendChild(header);
							
								form = document.createElement("form");
								form.setAttribute('id','advisorOptions');
							
								var tableTAG2 = document.createElement("table");
								tableTAG2.setAttribute('id','table2');
								tableTAG2.border = 1;
								tableTAG2.align = 'center';
								body.appendChild(tableTAG2);
								var hrow = tableTAG2.insertRow(0);
								hrow.align = 'center';
								var C = hrow.insertCell(0);
								var cellContents = document.createTextNode('Add Session');
								C.appendChild(cellContents);
							
							
								C = hrow.insertCell(1);
								input = document.createElement("input");
								input.setAttribute('type','radio');
								//alert(psid);
								//alert(access);
								input.setAttribute('onclick',"advisorAction('1','" + psid + "','" +access + "')");
								C.appendChild(input);
							
								var hrow = tableTAG2.insertRow(1);
								hrow.align = 'center';
								var C = hrow.insertCell(0);
								var cellContents = document.createTextNode('Review Sessions');
								C.appendChild(cellContents);
							
							
								C = hrow.insertCell(1);
								input = document.createElement("input");
								input.setAttribute('type','radio');
								input.setAttribute('onclick',"advisorAction('2','" + psid + "','" +access + "')");
							
								C.appendChild(input);
							
							
								var hrow = tableTAG2.insertRow(2);
								hrow.align = 'center';
								var C = hrow.insertCell(0);
								var cellContents = document.createTextNode('Add Notes');
								C.appendChild(cellContents);
							
							
								C = hrow.insertCell(1);
								input = document.createElement("input");
								input.setAttribute('type','radio');
								//alert(psid);
								//alert(access);
								input.setAttribute('onclick',"advisorAction('3','" + psid + "','" +access + "')");
								C.appendChild(input);
							
								var hrow = tableTAG2.insertRow(3);
								hrow.align = 'center';
								var C = hrow.insertCell(0);
								var cellContents = document.createTextNode('Review Notes');
								C.appendChild(cellContents);
							
							
								C = hrow.insertCell(1);
								input = document.createElement("input");
								input.setAttribute('type','radio');
								input.setAttribute('onclick',"advisorAction('4','" + psid + "','" +access + "')");
							
								C.appendChild(input);
						
							
								form.appendChild(tableTAG2);
							
								body.appendChild(form);
							
							
							}
						}
					}
               		else if ( type == 3 )
               		{
               			var ack = httpRequest.responseText;
               			
               			
               			alert(ack);
               			if (ack != "")
					    {
						   	var xml = httpRequest.responseXML;
               				var root = httpRequest.responseXML.documentElement;
               				
               				var rootattr = root.attributes;
               				var courses = rootattr.getNamedItem("courses").value;
               				var course = rootattr.getNamedItem("course").value;
               				var dept = rootattr.getNamedItem("dept").value;
               				//alert(courses)
               				
               				var oldTable = document.getElementById('theTable');
               				var body = document.getElementsByTagName("body")[0];
               				var header = document.getElementsByTagName("h1")[0];
               				var pollForm = document.getElementById('backToLogin');
               				
               				
               				var avggpa = root.getElementsByTagName('avggpa')[0].childNodes[0].nodeValue;

               				if ( oldTable != null )
               				{
               					body.removeChild(oldTable);
               					body.removeChild(header);
               					body.removeChild(pollForm);
               					
               				}
               				
               				header = document.createElement('h1');
               				header.align = 'center';
               				
               				text = document.createTextNode("Course Info: " + dept + course);
               				
               				
               				
               				
               				header.appendChild(text);
               				body.appendChild(header);
               				
               				header = document.createElement('h3');
               				header.align = 'center';
               				
               				text = document.createTextNode("Average GPA: " + avggpa);	
               				
               				header.appendChild(text);
               				body.appendChild(header);
               				
               				
               				var theTable = document.createElement('table');
               				theTable.setAttribute('id','theTable');
               				theTable.border = 1;
               				theTable.align = 'center';
               				body.appendChild(theTable);
               				var hrow = theTable.insertRow(0);
               				hrow.align = 'center';
               				var C = hrow.insertCell(0);
               				var cellContents = document.createTextNode('PeopleSoftID');
               				C.appendChild(cellContents);
               				
               				C = hrow.insertCell(1);
               				cellContents = document.createTextNode('Grade');
               				C.appendChild(cellContents);
               				
               				C = hrow.insertCell(2);
               				cellContents = document.createTextNode('GPA');
               				C.appendChild(cellContents);
               				
               				
						   	for (var i = 0; i < courses; i++)
						   	{
						   	
						   		hrow = theTable.insertRow(i+1);
               					hrow.align = 'center';
               					
               					contents = root.getElementsByTagName('psid')[i].childNodes[0];
               					if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
               					C = hrow.insertCell(0);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
               					
               					contents = root.getElementsByTagName('grade')[i].childNodes[0];
               					if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
               					C = hrow.insertCell(1);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
							
								contents = root.getElementsByTagName('gpa')[i].childNodes[0];
								if ( contents != null )
               					{
               						contents = contents.nodeValue;
               					}
               					else
               					{
               						contents = "";
               					}
               					
								C = hrow.insertCell(2);
								cellContents = document.createTextNode(contents);
								C.appendChild(cellContents);
								
								
								
							}
							
							form = createBackToLogin(body,psid,access);
							body.appendChild(form);
               				
							
						}
						else
						{
							//nothing?
						}
               		
               		}
               		
               }	
               
               
               
           }
           else
           {   
           		alert('Problem with request'); 
        	}
       }
	}
    
    function Login( psid, access )
    {
    	var bodyTAG = document.getElementsByTagName("body")[0];
    	var headerTAG = document.getElementsByTagName("h3")[0];
    	
    	
    	if ( headerTAG != null )
		{
			bodyTAG.removeChild(headerTAG);
			
		}
    	if ( access == 0 )
    	{
    			//alert(access);
    			var bodyTAG = document.getElementsByTagName("body")[0];
    			var formTAG = document.getElementsByTagName("form")[0];
    			var oldTable = document.getElementById('theTable');
    			
    			if ( oldTable != null )
				{
					bodyTAG.removeChild(oldTable);
					bodyTAG.removeChild(formTAG);
					
				}
				
				oldTable = document.getElementById('table2');
    			formTAG = document.getElementById('backToLogin');
    			if ( oldTable != null )
				{
					formTAG.removeChild(oldTable);
					bodyTAG.removeChild(formTAG);
					
				}
				
				var headerTAG = document.getElementsByTagName("h1")[0];
				if ( headerTAG != null )
				{
					bodyTAG.removeChild(headerTAG);
					
				}
    			
    			//alert(access);
    			
    				var formTAG = document.createElement('form');
    			
    				formTAG.setAttribute('name','pollForm');
    					var tableTAG = document.createElement('table');
    					tableTAG.setAttribute('id','theTable');
    					tableTAG.setAttribute('border','1');
    					tableTAG.align = 'center';
    						var captionTAG = document.createElement('caption');
    						captionTAG.align = 'center';
    						var captionContent = document.createTextNode("Choose one of the Following:");
    						captionTAG.appendChild(captionContent);
    					tableTAG.appendChild(captionTAG);
    					hrow = tableTAG.insertRow(0);
    					hrow.align = 'center';
    		
    					C = hrow.insertCell(0);
						cellContents = document.createTextNode("Choice");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						cellContents = document.createTextNode("Select");
						C.appendChild(cellContents);
						
						
						hrow = tableTAG.insertRow(1);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Show Grades by Term");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						var inputTAG = document.createElement('input');
						inputTAG.setAttribute('type','radio');
						inputTAG.setAttribute('name','options');
						inputTAG.setAttribute('value', '1');
						inputTAG.setAttribute('onclick', "processData(1, 1,'" + psid + "','" + access + "')");
						C.appendChild(inputTAG);
						
						
						hrow = tableTAG.insertRow(2);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Show Grades Alphabetically");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						var inputTAG = document.createElement('input');
						inputTAG.setAttribute('type','radio');
						inputTAG.setAttribute('name','options');
						inputTAG.setAttribute('value', '1');
						inputTAG.setAttribute('onclick', "processData(1, 2,'" + psid + "','" + access + "')");
						C.appendChild(inputTAG);
						
						hrow = tableTAG.insertRow(3);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Show Degree Progress");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						var inputTAG = document.createElement('input');
						inputTAG.setAttribute('type','radio');
						inputTAG.setAttribute('name','options');
						inputTAG.setAttribute('value', '1');
						inputTAG.setAttribute('onclick', "processData(1, 3,'" + psid + "','" + access + "')");
						C.appendChild(inputTAG);
						
					formTAG.appendChild(tableTAG);
				bodyTAG.appendChild(formTAG);

					form = createBackToLogin( bodyTAG, psid, access );
					bodyTAG.appendChild(form);
						
    					
    				
    	
    	}
    	else if ( access == 1 )
    	{
    		//alert(access);
    			var bodyTAG = document.getElementsByTagName("body")[0];
    			var formTAG = document.getElementsByTagName("form")[0];
    			var oldTable = document.getElementById('theTable');
    			
    			if ( oldTable != null )
				{
					bodyTAG.removeChild(oldTable);
					bodyTAG.removeChild(formTAG);
					
				}
				
				oldTable = document.getElementById('table2');
    			formTAG = document.getElementById('backToLogin');
    			if ( oldTable != null )
				{
					bodyTAG.removeChild(oldTable);
					bodyTAG.removeChild(formTAG);
					
				}
				
				var headerTAG = document.getElementsByTagName("h1")[0];
				if ( headerTAG != null )
				{
					bodyTAG.removeChild(headerTAG);
					
				}
    			
    			//alert(access);
    			
    				var formTAG = document.createElement('form');
    			
    				formTAG.setAttribute('name','pollForm');
    					var tableTAG = document.createElement('table');
    					tableTAG.setAttribute('id','theTable');
    					tableTAG.setAttribute('border','1');
    					tableTAG.align = 'center';
    						var captionTAG = document.createElement('caption');
    						captionTAG.align = 'center';
    						var captionContent = document.createTextNode("Choose one of the Following:");
    						captionTAG.appendChild(captionContent);
    					tableTAG.appendChild(captionTAG);
    					
    					hrow = tableTAG.insertRow(0);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Choice");
						C.appendChild(cellContents);
						
						C = hrow.insertCell(1);
						cellContents = document.createTextNode("Select");
						C.appendChild(cellContents);
    					
    					
						hrow = tableTAG.insertRow(1);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Search Student");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						var inputTAG = document.createElement('input');
						inputTAG.setAttribute('type','radio');
						inputTAG.setAttribute('name','options');
						inputTAG.setAttribute('value', '1');
						inputTAG.setAttribute('onclick', "processData(2, 1,'" + psid + "','" + access + "')");
						C.appendChild(inputTAG);
						
						hrow = tableTAG.insertRow(2);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Search Course");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						var inputTAG = document.createElement('input');
						inputTAG.setAttribute('type','radio');
						inputTAG.setAttribute('name','options');
						inputTAG.setAttribute('value', '1');
						inputTAG.setAttribute('onclick', "processData(3, 1,'" + psid + "','" + access + "')");
						C.appendChild(inputTAG);
						
					formTAG.appendChild(tableTAG);
				bodyTAG.appendChild(formTAG);
				
				form = createBackToLogin(bodyTAG, psid, access);
				bodyTAG.appendChild(form);
		
				
				
    	
    	}
    	else if ( access == 2 )
    	{
    		
    			var bodyTAG = document.getElementsByTagName("body")[0];
    			var formTAG = document.getElementsByTagName("form")[0];
    			var oldTable = document.getElementById('theTable');
    			
    			if ( oldTable != null )
				{
					bodyTAG.removeChild(oldTable);
					bodyTAG.removeChild(formTAG);
					
				}
				
				var headerTAG = document.getElementsByTagName("h1")[0];
				if ( headerTAG != null )
				{
					bodyTAG.removeChild(headerTAG);
					
				}
    			
    			//alert(access);
    			
    				var formTAG = document.createElement('form');
    			
    				formTAG.setAttribute('name','pollForm');
    					var tableTAG = document.createElement('table');
    					tableTAG.setAttribute('id','theTable');
    					tableTAG.setAttribute('border','1');
    					tableTAG.align = 'center';
    						var captionTAG = document.createElement('caption');
    						captionTAG.align = 'center';
    						var captionContent = document.createTextNode("Choose one of the Following:");
    						captionTAG.appendChild(captionContent);
    					tableTAG.appendChild(captionTAG);
    					bodyTAG.appendChild(tableTAG);
    					hrow = tableTAG.insertRow(0);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Choice");
						C.appendChild(cellContents);
						
						C = hrow.insertCell(1);
						cellContents = document.createTextNode("Select");
						C.appendChild(cellContents);
    					
    					
						hrow = tableTAG.insertRow(1);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Search Student");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						var inputTAG = document.createElement('input');
						inputTAG.setAttribute('type','radio');
						inputTAG.setAttribute('name','options');
						inputTAG.setAttribute('value', '1');
						inputTAG.setAttribute('onclick', "processData(2, 1,'" + psid + "','" + access + "')");
						C.appendChild(inputTAG);
						
						hrow = tableTAG.insertRow(2);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Search Course");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						var inputTAG = document.createElement('input');
						inputTAG.setAttribute('type','radio');
						inputTAG.setAttribute('name','options');
						inputTAG.setAttribute('value', '1');
						inputTAG.setAttribute('onclick', "processData(3, 1,'" + psid + "','" + access + "')");
						C.appendChild(inputTAG);
						
    					
						hrow = tableTAG.insertRow(3);
						hrow.align = 'center';
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Add/Remove User");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						var inputTAG = document.createElement('input');
						inputTAG.setAttribute('type','radio');
						inputTAG.setAttribute('name','options');
						inputTAG.setAttribute('value', '1');
						inputTAG.setAttribute('onclick', "addRemoveStudent('" + psid + "','" + access + "')");
						C.appendChild(inputTAG);
						
						hrow = tableTAG.insertRow(4);
						hrow.align = 'center';
						
						C = hrow.insertCell(0);
						cellContents = document.createTextNode("Upload Courses");
						C.appendChild(cellContents);
			
						C = hrow.insertCell(1);
						var inputTAG = document.createElement('input');
						inputTAG.setAttribute('type','radio');
						inputTAG.setAttribute('name','options');
						inputTAG.setAttribute('value', '1');
						inputTAG.setAttribute('onclick', "uploadCourse('" + psid + "','" + access + "')");
						C.appendChild(inputTAG);
						
					formTAG.appendChild(tableTAG);
				bodyTAG.appendChild(formTAG);
				
				form = createBackToLogin(bodyTAG, psid, access);
				bodyTAG.appendChild(form);
    	
    	}
    
    
    }

    
    
    function tableCreate()
    {
		var body = document.body,
		tbl = document.createElement('table');
		tbl.style.width='100%';
		tbl.style.border = "1px; solid; black;";

		for(var i = 0; i < 3; i++)
		{
			var tr = tbl.insertRow();
			for(var j = 0; j < 2; j++)
			{
				if(i==2 && j==1)
				{
					break
				} 
				else 
				{
					var td = tr.insertCell();
					td.appendChild(document.createTextNode('\u0020'))
					if(i==1&&j==1)
					{
						td.setAttribute('rowSpan','2');
					}
				}
			}
		}
		body.appendChild(tbl);
	}
	
	function getCourses ( access, psid )
	{
		if ( access == 0 )
		{
			var httpRequest;
			
			if (window.XMLHttpRequest) 
			{ // Mozilla, Safari, ...
				httpRequest = new XMLHttpRequest();
				if (httpRequest.overrideMimeType) 
				{
					httpRequest.overrideMimeType('text/xml');
				}
			}
			else if (window.ActiveXObject) 
			{ // IE
				try 
				{
					httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
				}
				catch (e) 
				{
					try 
					{
						httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
					}
					catch (e) {}
				}
			}
			if (!httpRequest) 
			{
				alert('Giving up :( Cannot create an XMLHTTP instance');
				return false;
			}
	
			var data;
			
			data = 'psid=' + psid;
			
	
			httpRequest.open('POST', 'getCourses.php',true);
			httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
			//httpRequest.setRequestHeader('Content-Type', 'text/xml');

			httpRequest.onreadystatechange = function() { addStudentCourses(httpRequest, psid); };

			httpRequest.send(data);
				
		
		}
		function Course(term, dept, classnum, grade, gpa )
		{
			this.term = term;
			this.dept = dept;
			this.classnum = classnum;
			this.grade = grade;
			this.gpa = gpa;
		
		}
		
		function addStudentCourses( httpRequest, psid )
		{
			if (httpRequest.readyState == 4)
        	{
           		if (httpRequest.status == 200)
           		{
           		
           			var ack = httpRequest.responseText;
           			alert(ack);
           			if ( ack != null )
           			{
           				var xml = httpRequest.responseXML;
           				var root = httpRequest.responseXML.documentElement;
               			var rootattr = root.attributes;
               			var courses = rootattr.getNamedItem("courses").value;
               			numCourses = courses;
               			
               			for ( i = 0; i < courses; i++ )
               			{
               				var course = root.getElementsByTagName("Course")[i];
               				
               				term = root.getElementsByTagName('Term')[i].childNodes[0].nodeValue;
               				dept = root.getElementsByTagName('Department')[i].childNodes[0].nodeValue;
               				classnum = root.getElementsByTagName('ClassNumber')[i].childNodes[0].nodeValue;
               				grade = root.getElementsByTagName('Grade')[i].childNodes[0].nodeValue;
               				gpa = root.getElementsByTagName('gpa')[i].childNodes[0].nodeValue;
               				studentCourseArray[i] = new Course(term, dept, classnum, grade, gpa );
               			
               			}
           			
           			}
           		
           		
           		}
           		
           	}
           		
		
		}
		
		
		
	
	}
	function addRemoveStudent(psid, access)
	{
		var httpRequest;
		
		if (window.XMLHttpRequest) 
		{ // Mozilla, Safari, ...
			httpRequest = new XMLHttpRequest();
			if (httpRequest.overrideMimeType) 
			{
				httpRequest.overrideMimeType('text/xml');
			}
		}
		else if (window.ActiveXObject) 
		{ // IE
			try 
			{
				httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch (e) 
			{
				try 
				{
					httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
				}
				catch (e) {}
			}
		}
		if (!httpRequest) 
		{
			alert('Giving up :( Cannot create an XMLHTTP instance');
			return false;
		}

		var data;
		
		

		httpRequest.open('POST', 'addRemove.php',true);
		httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		//httpRequest.setRequestHeader('Content-Type', 'text/xml');

		httpRequest.onreadystatechange = function() { showAddRemoveStudent(httpRequest, psid, access); };

		httpRequest.send(null);
		
		
		
	
	}
	
	function showAddRemoveStudent(httpRequest, psid, access)
	{
		
		if (httpRequest.readyState == 4)
		{
			if (httpRequest.status == 200)
			{
				var ack = httpRequest.responseText;
				
				if ( ack !=null )
				{
					
				/*here i will create a table with all the psids with associated radio buttons
					if the user clicks the radio button next to a user that user is removed ( given an access of -1 
					they cannon log on again)
					
					then i would have had a bunch of text fields for all the user fields to fill in then a submit radio button*/
					//alert(ack);
		
					var ack = httpRequest.responseTextXML;
					var root = httpRequest.responseXML.documentElement;
					var rootattr = root.attributes;
					var users = rootattr.getNamedItem("users").value;
					
					
					var oldTable = document.getElementById('theTable');
					var formTAG2 = document.getElementById("backToLogin");
					var body = document.getElementsByTagName("body")[0];
					var poll = body.getElementsByTagName("form")[0];
					if ( oldTable != null )
					{
						poll.removeChild(oldTable);
						body.removeChild(formTAG2);
					}
		
					header = document.createElement('h1');
					header.align = 'center';

					text = document.createTextNode('Add/Remove Student');
					header.appendChild(text);
					body.appendChild(header);
					
					
					
					
					
					var theTable = document.createElement('table');
					theTable.setAttribute('id','theTable');
					theTable.border = 1;
					theTable.align = 'center';
					body.appendChild(theTable);
					var hrow = theTable.insertRow(0);
					hrow.align = 'center';
					var C = hrow.insertCell(0);
					var cellContents = document.createTextNode('Psid');
					C.appendChild(cellContents);
	
					C = hrow.insertCell(1);
					var cellContents = document.createTextNode('Remove');
					C.appendChild(cellContents);
					
					
					for ( i =0; i < users; i++ )
					{
						
						var hrow = theTable.insertRow(i+1);
						hrow.align = 'center';

						var C = hrow.insertCell(0);
						peoplesoft = root.getElementsByTagName('psid')[i].childNodes[0].nodeValue;
						text = document.createTextNode(peoplesoft);
						C.appendChild(text);

						C = hrow.insertCell(1);

						input = document.createElement("input");
						input.setAttribute('type','radio');
						input.setAttribute('name','peoplesoft');
						input.setAttribute('value', "");
						input.setAttribute('onclick',"removeUser('" + psid + "','" + access +"','" + peoplesoft +"')");
						C.appendChild(input);
						
					}
					
					
					
				}
			}
			
		}
	
	}
	
	function removeUser ( psid, access, peoplesoft )
	{
		var httpRequest;
			
			if (window.XMLHttpRequest) 
			{ // Mozilla, Safari, ...
				httpRequest = new XMLHttpRequest();
				if (httpRequest.overrideMimeType) 
				{
					httpRequest.overrideMimeType('text/xml');
				}
			}
			else if (window.ActiveXObject) 
			{ // IE
				try 
				{
					httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
				}
				catch (e) 
				{
					try 
					{
						httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
					}
					catch (e) {}
				}
			}
			if (!httpRequest) 
			{
				alert('Giving up :( Cannot create an XMLHTTP instance');
				return false;
			}
	
			var data;
			
			data = 'remove=' + 1 + '&' + 'psid=' + peoplesoft;
			
	
			httpRequest.open('POST', 'getCourses.php',true);
			httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
			//httpRequest.setRequestHeader('Content-Type', 'text/xml');

			httpRequest.onreadystatechange = function() { confirmRemove(httpRequest, psid); };

			httpRequest.send(data);
				
	
	}
		
	function confirmRemove(httpRequest, psid )
	{
		if (httpRequest.readyState == 4)
		{
			if (httpRequest.status == 200)
			{
			
				ack = httpRequest.responseText;
				
				alert(ack);
			
			
			}
			
		}
	
	
	}
	function addUser ( psid, access, peoplesoft, first, last, pass, email, userid )
	{
		var httpRequest;
			
			if (window.XMLHttpRequest) 
			{ // Mozilla, Safari, ...
				httpRequest = new XMLHttpRequest();
				if (httpRequest.overrideMimeType) 
				{
					httpRequest.overrideMimeType('text/xml');
				}
			}
			else if (window.ActiveXObject) 
			{ // IE
				try 
				{
					httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
				}
				catch (e) 
				{
					try 
					{
						httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
					}
					catch (e) {}
				}
			}
			if (!httpRequest) 
			{
				alert('Giving up :( Cannot create an XMLHTTP instance');
				return false;
			}
	
			var data;
			
			data = 'add=' + 1 + '&' + 'psid=' + peoplesoft + '&' + 'first=' + first + '&' + 'last=' + last + '&' + 'pass=' + pass + '&' + 'email=' + email + '&' + 'user=' + userid;
			
	
			httpRequest.open('POST', 'getCourses.php',true);
			httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
			//httpRequest.setRequestHeader('Content-Type', 'text/xml');

			//httpRequest.onreadystatechange = function() { addStudentCourses(httpRequest, psid); };

			httpRequest.send(data);
				
	
	}
	
	function uploadCourses(psid, access)
	{
		var oldTable = document.getElementById('theTable');
		var formTAG2 = document.getElementById("backToLogin");
		var body = document.getElementsByTagName("body")[0];
		var poll = body.getElementsByTagName("form")[0];
		if ( oldTable != null )
		{
			poll.removeChild(oldTable);
			body.removeChild(formTAG2);
		}
		
		header = document.createElement('h1');
		header.align = 'center';

		text = document.createTextNode('Upload Courses');

		header.appendChild(text);
		body.appendChild(header);
		
		
		
		input = document.createElement("input");
		input.align = 'center';
		input.setAttribute('type','text');
		input.setAttribute('name','file');
		

		body.appendChild(input);
		
		header = document.createElement('h3');
		header.align = 'center';

		text = document.createTextNode('Submit File');

		header.appendChild(text);
		body.appendChild(header);
		input = document.createElement("input");
		input.align = 'center';
		input.setAttribute('type','radio');

		input.setAttribute('onclick',"addCourseFile('" + psid + "','" + access +"')");

		body.appendChild(input);
		
		form = createBackToLogin(body, psid, access);
		body.appendChild(form);

	
	
	}
	
	function addCourseFile(psid, access)
	{
		
		file = document.getElementsByName('file')[0].value;
		
		
		var httpRequest;
		
		if (window.XMLHttpRequest) 
		{ // Mozilla, Safari, ...
			httpRequest = new XMLHttpRequest();
			if (httpRequest.overrideMimeType) 
			{
				httpRequest.overrideMimeType('text/xml');
			}
		}
		else if (window.ActiveXObject) 
		{ // IE
			try 
			{
				httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch (e) 
			{
				try 
				{
					httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
				}
				catch (e) {}
			}
		}
		if (!httpRequest) 
		{
			alert('Giving up :( Cannot create an XMLHTTP instance');
			return false;
		}

		var data;
		
		data = 'file=' + file;

		httpRequest.open('POST', 'addCourses.php',true);
		httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		//httpRequest.setRequestHeader('Content-Type', 'text/xml');

		httpRequest.onreadystatechange = function() { coursesAdded(httpRequest, psid, access); };

		httpRequest.send(data);
		
	
	}
	
	function coursesAdded( httpRequest, psid, access )
	{
	
		if (httpRequest.readyState == 4)
		{
			if (httpRequest.status == 200)
			{
			
			
				var body = document.getElementsByTagName("body")[0];
				var poll = document.getElementsByTagName("form")[0];
				var input1 = document.getElementsByTagName("input")[0];
				var input2 = document.getElementsByTagName("input")[1];
				var pollForm = document.getElementById('backToLogin');
				
				body.removeChild(input1);
				body.removeChild(input2);
				body.removeChild(poll);
				body.removeChild(pollForm);
		
				
				alert(httpRequest.responseText);
				
				
				
				Login(psid, access);
			
			}
			
		}
	
	}
	
	function changePass(psid, access)
	{
	
		
		var oldTable = document.getElementById('theTable');
		var formTAG2 = document.getElementById("backToLogin");
		var body = document.getElementsByTagName("body")[0];
		var poll = body.getElementsByTagName("form")[0];
		if ( oldTable != null )
		{
			poll.removeChild(oldTable);
			body.removeChild(formTAG2);
		}
	
		var theTable = document.createElement('table');
		theTable.setAttribute('id','theTable');
		theTable.border = 1;
		theTable.align = 'center';
		body.appendChild(theTable);
		var hrow = theTable.insertRow(0);
		hrow.align = 'center';
		var C = hrow.insertCell(0);
		var cellContents = document.createTextNode('Old Password');
		C.appendChild(cellContents);
	
		C = hrow.insertCell(1);
	
		input = document.createElement("input");
		input.setAttribute('type','password');
		input.setAttribute('name','old');
		input.setAttribute('value', "");
		C.appendChild(input);
	
		var hrow = theTable.insertRow(1);
		hrow.align = 'center';
	
		var C = hrow.insertCell(0);
		var cellContents = document.createTextNode('New Password');
		C.appendChild(cellContents);
	
		C = hrow.insertCell(1);
	
		input = document.createElement("input");
		input.setAttribute('type','password');
		input.setAttribute('name','new');
		input.setAttribute('value', "");
		C.appendChild(input);
		
		var hrow = theTable.insertRow(2);
		hrow.align = 'center';
	
		var C = hrow.insertCell(0);
		var cellContents = document.createTextNode('Submit');
		C.appendChild(cellContents);
	
		C = hrow.insertCell(1);
	
		input = document.createElement("input");
		input.setAttribute('type','radio');
		
		input.setAttribute('id', 'submitted');
		input.setAttribute('onclick',"newPass('" + psid + "','" + access +"')");
		
		C.appendChild(input);
	
		
		form = createBackToLogin(body, psid, access);
		body.appendChild(form);
		
		
		
	
	
	}
	
	function newPass(psid, access )
	{
		oldPass = document.getElementsByName('old')[0].value;
		newPass = document.getElementsByName('new')[0].value;
		
		
		var httpRequest;
		
		if (window.XMLHttpRequest) 
		{ // Mozilla, Safari, ...
			httpRequest = new XMLHttpRequest();
			if (httpRequest.overrideMimeType) 
			{
				httpRequest.overrideMimeType('text/xml');
			}
		}
		else if (window.ActiveXObject) 
		{ // IE
			try 
			{
				httpRequest = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch (e) 
			{
				try 
				{
					httpRequest = new ActiveXObject("Microsoft.XMLHTTP");
				}
				catch (e) {}
			}
		}
		if (!httpRequest) 
		{
			alert('Giving up :( Cannot create an XMLHTTP instance');
			return false;
		}

		var data;
		
		data = 'oldPass=' + oldPass + '&' + 'newPass=' + newPass + '&' + 'psid=' + psid;
		

		httpRequest.open('POST', 'changePass.php',true);
		httpRequest.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		//httpRequest.setRequestHeader('Content-Type', 'text/xml');

		httpRequest.onreadystatechange = function() { showPassResult(httpRequest, psid, access); };

		httpRequest.send(data);
		
		
	
	}
	
	function showPassResult( httpRequest, psid, access )
	{
		if (httpRequest.readyState == 4)
		{
			if (httpRequest.status == 200)
			{
			
				alert(httpRequest.responseText);
				
				Login(psid, access);
			
			}
			
		}
	
	
	}
</script>
<?php
	if (isset($_SESSION['psid'] )&& isset($_SESSION['access'] ) ) 
	{
		$psid = $_SESSION['psid'];
		$access = $_SESSION['access'];
		echo "<body onload = 'getCourses($access, $psid)'>";
?>

	<form name = "pollForm">
	<table id = "theTable" border = "1" align = 'center'>
		<caption>Choose one of the Following:</caption>
		<tr align = 'center'>
			<td>Choice</td><td>Select</td>
		</tr>
		<tr align = 'center'>
		
	<?php
		if ( $access == 0 )
		{
	
			echo "<td>Show Grades by Term</td>";
			echo "<td><input type = 'radio' name = 'options' 
						   value = '1'
						   onclick = 'processData(1, 1, $psid, $access)'></td>";
		}
		else
		{
			echo "<td>Search Student</td>";
			echo "<td><input type = 'radio' name = 'options' 
						   value = '1'
						   onclick = 'processData(2, 1, $psid, $access)'></td>";
		
		}
	?>
		</tr>
		<tr align = 'center'>
	<?php
		if ( $access == 0 )
		{

			echo "<td>Show Grades Alphabetically</td>";
			echo "<td><input type = 'radio' name = 'options' 
					   value = '1'
					   onclick = 'processData(1, 2, $psid, $access)'></td>";
		}
		else
		{
			echo "<td>Search Course</td>";
			echo "<td><input type = 'radio' name = 'options' 
					   value = '1'
					   onclick = 'processData(3, 1, $psid, $access)'></td>";

		}
		if ( $access == 0 )
		{
	?>
		</tr>
		<tr align = 'center'>
			<td>Show Requirements List</td>
			<?php
				echo "<td><input type = 'radio' name = 'options' 
						   value = '3'
						   onclick = 'processData(1, 3, $psid, $access)'></td>";
			?>
		</tr>
	<?php
		}
		elseif( $access == 2 )
		{
	?>
		
			<tr align = 'center'>
			<td>Add/Remove Student</td>
			<?php
				echo "<td><input type = 'radio' name = 'options' 
						   value = '3'
						   onclick = 'addRemoveStudent($psid,$access)'></td>";
			?>
		</tr>
		
		<tr align = 'center'>
			<td>Upload Courses</td>
			<?php
				echo "<td><input type = 'radio' name = 'options' 
						   value = '3'
						   onclick = 'uploadCourses($psid,$access)'></td>";
			?>
		</tr>
	<?php
		}
	?>
	</table>
	</form>
	
	<form id="backToLogin">
	<table id="table2" border='1' align='center'>
	
	<tr align = 'center'>
			<td>Back to Menu</td>
			<?php
				$id = "table2";
				echo "<td><input type = 'radio' name = 'menu' 
						   value = '3'
						   onclick = 'Login($psid, $access)'
						   onmouseover ="."showSelected(0,'table2')"."
						   onmouseout =". "makeRegular(0,'table2')"." ></td>";
			?>
	</tr>
	<tr align = 'center'>
			<td>Change Password</td>
			<?php
				$id = "table2";
				echo "<td><input type = 'radio' name = 'menu' 
						   value = '3'
						   onclick = 'changePass($psid, $access)'
						   onmouseover ="."showSelected(1,'table2')"."
						   onmouseout =". "makeRegular(1,'table2')"." ></td>";
			?>
	</tr>
	
	<tr align = 'center'>
			<td>Logout</td>
			<?php
				echo "<td><input type = 'radio' name = 'menu' 
						   value = '3'
						   onclick = 'Logout()'
						   onmouseover ="."showSelected(2,'table2')"."
						   onmouseout =". "makeRegular(2,'table2')"." ></td>";
						   ;
			?>
	</tr>
	</table>
	</form>	
	
	</body>
	</html>

<?php

	}
	
	session_destroy();
?>
	
	