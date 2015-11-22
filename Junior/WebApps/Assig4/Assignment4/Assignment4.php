<html>
<head>
<link rel = "stylesheet" type = "text/css" href = "A3Style.css"/>
<title>Assignment 4</title>
<script src="http://code.jquery.com/jquery-latest.js"></script>
<script type="text/JavaScript" src="jsDraw2D.js"></script>
<script type="text/javascript">
var currentWord = null, lettersGuessed, numGuessed=0, numWrong = 0, wordSize, guessedWord, win = 0;
var totalWon=0, totalLost=0, wordsSeen, didConfirm = -1;

//Andrew Beers Assignment 4 CS 1520 7/22

//used to replace _ in the current word with the letter that was correctly guessed
function setCharAt(str,index,chr) 
{
    if(index > str.length-1)
    {
    	result = str;
    }
    else
    {
    	result = str.substr(0,index) + chr + str.substr(index+1);
    }
    
    return result;
}

//used to grab a new word for the game, and reset all variables to get a fresh round
function resetRound()
{
	if ( currentWord == null )
	{
		$.post("newWord.php",null, function(data)
		{
			wordsSeen = new Array();
			lettersGuessed = new Array();
			//remove old stuff from current game;
			currentWord = $(data).find("name").text();
			//alert(currentWord);
			wordSize = currentWord.length;
			guessedWord = "";
			wordsSeen.push(currentWord);
		
			for ( i = 0; i < wordSize; i++ )
			{
				guessedWord += "_ ";
		
			}
		
			//initially create table
			row = $("table#hangman tr").eq(0);

			newE = "</tr><tr>";
			newE += "<td>Hangman Word</td><td>Guess a Letter</td>";
			newE += "</tr>";
			newE +="<tr>";
			newE +="<td id = 'answer'>" + guessedWord + "</td><td><input type ='text' id = 'letter' value =''></input></td>";
			newE +="</tr>";
		
			newE +="<tr><td id = 'incorrect'>Incorrect: 0 Total: 0</td><td><input type ='button' id ='submitLetter' onclick ='checkLetter()' value ='Guess Letter'></td>";
		
			newE +="</tr><tr><td>Letters Guessed</td><td id ='guessedLetters'> </td>";
		
			row.append(newE);
				
				
			
		
		});
	}
	else
	{
		var newWordFound = false;
		//if the player has not won or lost, confirm their decision to start a new round
		if ( win == 0 && didConfirm == -1)
		{
			didConfirm = confirm("Are you sure?");
		}
		
		//get a new word for the game
		if ( (win != 0 || didConfirm) && !newWordFound )
		{
			$.post("newWord.php",null, function(data)
			{
				win = 0;
				lettersGuessed = new Array();
				currentWord = $(data).find("name").text();
				
				if ( $.inArray(currentWord, wordsSeen) == -1 )
				{
					//alert(currentWord);
					wordSize = currentWord.length;
					guessedWord = "";
					//reset drawing
					$("#canvas").text("");
				
		
					for ( i = 0; i < wordSize; i++ )
					{
						guessedWord += "_ ";
		
					}
			
					$("#answer").text(guessedWord);
			
					$("#guessedLetters").text(" ");
			
					$("#incorrect").text("Incorrect: 0 Total: 0");
			
					$("#answer").text(guessedWord);
			
					numWrong = 0;
					numGuessed = 0;
					wordsSeen.push(currentWord);
					newWordFound = true
					didConfirm == -1;
				}
				else
				{
					alert(currentWord + " has been used already, getting new word. " );
					resetRound();
				}
			
		
		
			});
		}
	}

}

//check if a letter is valid, and if it is in the word, then update the guessed word
function checkLetter()
{
	if ( win == 0)
	{
		guess = $("#letter").val();
	
		if ( guess.length > 1 )
		{
			guess = guess.charAt(0);
	
		}
		guess = guess.toLowerCase();

		if ( guess < "a" || guess > "z") 
		{
			guess = 0;
		}
	
		if ( guess != 0 )
		{
			//check if this letter has been guessed before
			if ( $.inArray(guess, lettersGuessed) == -1 ) 
			{
				numGuessed++;
				lettersGuessed.push(guess);
	
				currLetters = $("#guessedLetters").text();
				newGuessedLetters = currLetters + guess + ",";
				$("#guessedLetters").text(newGuessedLetters);
			
				currAnswer = $("#answer").text();
				found = 0;
				//check all indicies
				for (i = 0; i < wordSize; i++ ) 
				{
					if ( currentWord.charAt(i) == guess )
					{
						currAnswer = setCharAt(currAnswer,2*i, guess);
						found = 1;
					}
			   
				}
				$("#answer").text(currAnswer);
			
				if ( found == 0 )
				{
					numWrong++;
					$("#incorrect").text("Incorrect: " + numWrong + " Total: " + numGuessed);
					drawHangman(numWrong);
				
					if ( numWrong == 7 )
					{
						win = -1;
						totalLost++;
						totalPlayed = totalLost + totalWon;
						winP = totalWon / totalPlayed;
						winPercentage = winP * 100;
						alert("You lost");
						alert("Stats:\nTotal - " + totalPlayed + "\nWon - " + totalWon + "\nWin% - " + winPercentage + "%");
						
					}
				}
				else
				{
					$("#incorrect").text("Incorrect: " + numWrong + " Total: " + numGuessed);
					var match = currAnswer.match(/_/);
					if ( !match )
					{
						win = 1;
						totalWon++;
						totalPlayed = totalLost + totalWon;
						winP = totalWon / totalPlayed;
						winPercentage = winP * 100;
						alert("You won");
						alert("Stats:\nTotal - " + totalPlayed + "\nWon - " + totalWon + "\nWin% - " + winPercentage + "%");
						
					}
			
				}
			
			}
			else
			{
				alert("You have already guessed " + guess + ". Try a different letter.");
			}
		}
		else
		{
			alert("You have entered a non letter. Please enter a letter.");

	
		}
		
	}
	else
	{
		if ( win == 1 )
		{
			alert("You already won, click Start a New Round to play again.");
		}
		else
		{
			alert("You already lost, click Start a New Round to play again.");
		}
	
	
	}

	//clear out the guessing text box and focus on it
	$("#letter").val("");
	$("#letter").focus();


}

//drawing the hangman graphically
function drawHangman(numWrong)
{
	if( numWrong == 1 )
	{
		drawHead();
	}
	else if( numWrong == 2 )
	{
		drawBody();
	}
	else if( numWrong == 3 )
	{
		drawRightArm();
	}
	else if( numWrong == 4 )
	{
		drawLeftArm();
	}
	else if( numWrong == 5 )
	{
		drawRightLeg();
	}
	else if( numWrong == 6 )
	{
		drawLeftLeg();
	}
	else if( numWrong == 7 )
	{
		drawHat();
	}
}


</script>
</head>
<body>
<h1>Welcome to Hangman!</h1>
<table id= "hangman" border = '1' align ='center'>
<tr><td><input type='button' value ='Start a New Round' onclick = 'resetRound()'>
</table> 
<div id="canvas" style="position:relative;width:800px;height:300px;"></div>
<script type="text/JavaScript">

    //drawing objects 
    var gr = new jsGraphics(document.getElementById("canvas"));

    var blue = new jsColor("#057FE3");
    var black = new jsColor("#000000");
    var red = new jsColor("#FA0526");
    var gray = new jsColor("#666666");
    var white = new jsColor("#FFFFFF");

    var pen = new jsPen(blue,1);
    var redPen = new jsPen(red,1);
    
    //drawing functions
    function drawHead()
    {
    	var pt1 = new jsPoint(375,70);
    	gr.fillCircle(gray,pt1,30);
    	
    	pt1 = new jsPoint(385,65);
    	gr.fillCircle(black,pt1,5);
    	
    	pt1 = new jsPoint(365,65);
    	gr.fillCircle(black,pt1,5);
    }
    
    function drawBody()
    {
    	var pt1 = new jsPoint(375,100);
    	var pt2 = new jsPoint(375,200);
    	gr.drawLine(pen,pt1,pt2);
    
    }
    
    function drawRightArm()
    {
    	var pt1 = new jsPoint(375,155);
    	var pt2 = new jsPoint(425,120);
    	gr.drawLine(pen,pt1,pt2);
    }
    
    function drawLeftArm()
    {
    	var pt1 = new jsPoint(375,155);
    	var pt2 = new jsPoint(325,120);
    	gr.drawLine(pen,pt1,pt2);
    
    }
    
    function drawRightLeg()
    {
    	var pt1 = new jsPoint(375,200);
    	var pt2 = new jsPoint(425,275);
    	gr.drawLine(pen,pt1,pt2);
    }
    
    function drawLeftLeg()
    {
    	var pt1 = new jsPoint(375,200);
    	var pt2 = new jsPoint(325,275);
    	gr.drawLine(pen,pt1,pt2);
    }
    
    function drawHat()
    {
    	var pt1 = new jsPoint(310,40);
    	var pt2 = new jsPoint(440,40);
    	gr.drawLine(redPen,pt1,pt2);
    	
    	var pt1 = new jsPoint(340,40);
    	var pt2 = new jsPoint(340,5);
    	gr.drawLine(redPen,pt1,pt2);
    	
    	var pt1 = new jsPoint(410,40);
    	var pt2 = new jsPoint(410,5);
    	gr.drawLine(redPen,pt1,pt2);
    	
    	var pt1 = new jsPoint(340,5);
    	var pt2 = new jsPoint(410,5);
    	gr.drawLine(redPen,pt1,pt2);
    	
    	
    
    }
   
</script>
</body>
</html>