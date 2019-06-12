package edu.afit.scpsolver.parse;

public interface SCPParserConstants {
	int EOF = 0;
	int INT_LITERAL = 6;
	int DEFAULT = 0;
	String[] tokenImage = new String[]{"<EOF>", "\" \"", "\"\\t\"", "\"\\n\"", "\"\\r\"", "\"\\f\"", "<INT_LITERAL>",
			"\"{\"", "\"}\"", "\"(\"", "\",\"", "\")\""};
}