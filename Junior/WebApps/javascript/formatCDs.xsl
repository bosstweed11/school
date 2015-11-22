<?xml version="1.0" encoding="ISO-8859-1"?>
<!--CS 1520 Summer 2012
    Simple XSL document to transform the new rows of CDs from XML into 
    an HTML table.  See also CDpoll-xsl.php

    Note the XSLT and Xpath used to incorporate the XML contents in the
    resulting document can add function calls in addition to just simple
    data.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"/>
<xsl:template match="/CDResponse">
    <table id = "theTable" border = "1" class="thetable">
    <caption class="title">Select your Favorite CD<br/></caption>
    <tr align = "center">
        <th>Title</th><th>Artist</th><th>Count</th><th>Select</th>
    </tr>
    <xsl:for-each select="CD">
	<tr align='center' class='regular'>
	    <td>
	        <xsl:value-of select="Title"/>
	    </td>
	    <td>
	        <xsl:value-of select="Artist"/>
	    </td>
	    <td>
	        <xsl:value-of select="Votes"/>
	    </td>  
            <td>
		<input type = "radio" name = "options">
	 	<xsl:attribute name="value">
	        	<xsl:value-of select="id"/>
		</xsl:attribute>

	 	<xsl:attribute name="onclick">processData(1, <xsl:value-of select="id"/>)</xsl:attribute>
	 	<xsl:attribute name="onmouseover">showSelected(<xsl:value-of select="id"/>)</xsl:attribute>
	 	<xsl:attribute name="onmouseout">makeRegular(<xsl:value-of select="id"/>)</xsl:attribute>
                </input>
	    </td>
	</tr>
	</xsl:for-each>
   </table>
</xsl:template>
</xsl:stylesheet>
