<?xml version="1.0" encoding="ISO-8859-1"?>
<!--CS 1520 Summer 2012
    Simple XSL document to transform an XML RSS feed into a viewable
    HTML table
    Compare this with the version that uses DOM to create / format the table.
    It could be argued that this version is much simpler and more elegant than
    the straight DOM version
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"/>
<xsl:template match="/rss/channel">
  <h2>Liftoff News</h2>
  <table align = "center" border="1" id = "rssTable">
	<tr align="center">
		<th>title</th>
		<th>link</th>
		<th>description</th>
		<th>pubDate</th>
	</tr>
	<xsl:for-each select="item">
	<tr align="center">
		<td>
	            <xsl:value-of select="title"/>
	    </td>   
		<td>
		    <a>
	 			<xsl:attribute name="href">
		        	<xsl:value-of select="link"/>
				</xsl:attribute>
	   	    	<xsl:value-of select="link"/>
            </a>
		</td>
		<td>
	            <xsl:value-of select="description"/>
	    </td>   
		<td>
	            <xsl:value-of select="pubDate"/>
	    </td>   
	</tr>
	</xsl:for-each>
  </table>
</xsl:template>
</xsl:stylesheet>
