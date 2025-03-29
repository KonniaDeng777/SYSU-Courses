// Generated from /workspaces/SYsU-lang2/task/1/antlr/SYsU_lang.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue", "this-escape"})
public class SYsU_lang extends Lexer {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		Int=1, Return=2, LeftParen=3, RightParen=4, LeftBracket=5, RightBracket=6, 
		LeftBrace=7, RightBrace=8, Plus=9, Semi=10, Comma=11, Equal=12, Identifier=13, 
		Constant=14, LineAfterPreprocessing=15, Whitespace=16, Newline=17;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"Int", "Return", "LeftParen", "RightParen", "LeftBracket", "RightBracket", 
			"LeftBrace", "RightBrace", "Plus", "Semi", "Comma", "Equal", "Identifier", 
			"IdentifierNondigit", "Nondigit", "Digit", "Constant", "IntegerConstant", 
			"DecimalConstant", "OctalConstant", "NonzeroDigit", "OctalDigit", "LineAfterPreprocessing", 
			"Whitespace", "Newline"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'int'", "'return'", "'('", "')'", "'['", "']'", "'{'", "'}'", 
			"'+'", "';'", "','", "'='"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "Int", "Return", "LeftParen", "RightParen", "LeftBracket", "RightBracket", 
			"LeftBrace", "RightBrace", "Plus", "Semi", "Comma", "Equal", "Identifier", 
			"Constant", "LineAfterPreprocessing", "Whitespace", "Newline"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public SYsU_lang(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "SYsU_lang.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\u0004\u0000\u0011\u0097\u0006\uffff\uffff\u0002\u0000\u0007\u0000\u0002"+
		"\u0001\u0007\u0001\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002"+
		"\u0004\u0007\u0004\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002"+
		"\u0007\u0007\u0007\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002"+
		"\u000b\u0007\u000b\u0002\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e"+
		"\u0002\u000f\u0007\u000f\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011"+
		"\u0002\u0012\u0007\u0012\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014"+
		"\u0002\u0015\u0007\u0015\u0002\u0016\u0007\u0016\u0002\u0017\u0007\u0017"+
		"\u0002\u0018\u0007\u0018\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000"+
		"\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001"+
		"\u0001\u0001\u0001\u0002\u0001\u0002\u0001\u0003\u0001\u0003\u0001\u0004"+
		"\u0001\u0004\u0001\u0005\u0001\u0005\u0001\u0006\u0001\u0006\u0001\u0007"+
		"\u0001\u0007\u0001\b\u0001\b\u0001\t\u0001\t\u0001\n\u0001\n\u0001\u000b"+
		"\u0001\u000b\u0001\f\u0001\f\u0001\f\u0005\fV\b\f\n\f\f\fY\t\f\u0001\r"+
		"\u0001\r\u0001\u000e\u0001\u000e\u0001\u000f\u0001\u000f\u0001\u0010\u0001"+
		"\u0010\u0001\u0011\u0001\u0011\u0003\u0011e\b\u0011\u0001\u0012\u0001"+
		"\u0012\u0005\u0012i\b\u0012\n\u0012\f\u0012l\t\u0012\u0001\u0013\u0001"+
		"\u0013\u0005\u0013p\b\u0013\n\u0013\f\u0013s\t\u0013\u0001\u0014\u0001"+
		"\u0014\u0001\u0015\u0001\u0015\u0001\u0016\u0001\u0016\u0005\u0016{\b"+
		"\u0016\n\u0016\f\u0016~\t\u0016\u0001\u0016\u0005\u0016\u0081\b\u0016"+
		"\n\u0016\f\u0016\u0084\t\u0016\u0001\u0016\u0001\u0016\u0001\u0017\u0004"+
		"\u0017\u0089\b\u0017\u000b\u0017\f\u0017\u008a\u0001\u0017\u0001\u0017"+
		"\u0001\u0018\u0001\u0018\u0003\u0018\u0091\b\u0018\u0001\u0018\u0003\u0018"+
		"\u0094\b\u0018\u0001\u0018\u0001\u0018\u0000\u0000\u0019\u0001\u0001\u0003"+
		"\u0002\u0005\u0003\u0007\u0004\t\u0005\u000b\u0006\r\u0007\u000f\b\u0011"+
		"\t\u0013\n\u0015\u000b\u0017\f\u0019\r\u001b\u0000\u001d\u0000\u001f\u0000"+
		"!\u000e#\u0000%\u0000\'\u0000)\u0000+\u0000-\u000f/\u00101\u0011\u0001"+
		"\u0000\u0006\u0003\u0000AZ__az\u0001\u000009\u0001\u000019\u0001\u0000"+
		"07\u0002\u0000\n\n\r\r\u0002\u0000\t\t  \u0098\u0000\u0001\u0001\u0000"+
		"\u0000\u0000\u0000\u0003\u0001\u0000\u0000\u0000\u0000\u0005\u0001\u0000"+
		"\u0000\u0000\u0000\u0007\u0001\u0000\u0000\u0000\u0000\t\u0001\u0000\u0000"+
		"\u0000\u0000\u000b\u0001\u0000\u0000\u0000\u0000\r\u0001\u0000\u0000\u0000"+
		"\u0000\u000f\u0001\u0000\u0000\u0000\u0000\u0011\u0001\u0000\u0000\u0000"+
		"\u0000\u0013\u0001\u0000\u0000\u0000\u0000\u0015\u0001\u0000\u0000\u0000"+
		"\u0000\u0017\u0001\u0000\u0000\u0000\u0000\u0019\u0001\u0000\u0000\u0000"+
		"\u0000!\u0001\u0000\u0000\u0000\u0000-\u0001\u0000\u0000\u0000\u0000/"+
		"\u0001\u0000\u0000\u0000\u00001\u0001\u0000\u0000\u0000\u00013\u0001\u0000"+
		"\u0000\u0000\u00037\u0001\u0000\u0000\u0000\u0005>\u0001\u0000\u0000\u0000"+
		"\u0007@\u0001\u0000\u0000\u0000\tB\u0001\u0000\u0000\u0000\u000bD\u0001"+
		"\u0000\u0000\u0000\rF\u0001\u0000\u0000\u0000\u000fH\u0001\u0000\u0000"+
		"\u0000\u0011J\u0001\u0000\u0000\u0000\u0013L\u0001\u0000\u0000\u0000\u0015"+
		"N\u0001\u0000\u0000\u0000\u0017P\u0001\u0000\u0000\u0000\u0019R\u0001"+
		"\u0000\u0000\u0000\u001bZ\u0001\u0000\u0000\u0000\u001d\\\u0001\u0000"+
		"\u0000\u0000\u001f^\u0001\u0000\u0000\u0000!`\u0001\u0000\u0000\u0000"+
		"#d\u0001\u0000\u0000\u0000%f\u0001\u0000\u0000\u0000\'m\u0001\u0000\u0000"+
		"\u0000)t\u0001\u0000\u0000\u0000+v\u0001\u0000\u0000\u0000-x\u0001\u0000"+
		"\u0000\u0000/\u0088\u0001\u0000\u0000\u00001\u0093\u0001\u0000\u0000\u0000"+
		"34\u0005i\u0000\u000045\u0005n\u0000\u000056\u0005t\u0000\u00006\u0002"+
		"\u0001\u0000\u0000\u000078\u0005r\u0000\u000089\u0005e\u0000\u00009:\u0005"+
		"t\u0000\u0000:;\u0005u\u0000\u0000;<\u0005r\u0000\u0000<=\u0005n\u0000"+
		"\u0000=\u0004\u0001\u0000\u0000\u0000>?\u0005(\u0000\u0000?\u0006\u0001"+
		"\u0000\u0000\u0000@A\u0005)\u0000\u0000A\b\u0001\u0000\u0000\u0000BC\u0005"+
		"[\u0000\u0000C\n\u0001\u0000\u0000\u0000DE\u0005]\u0000\u0000E\f\u0001"+
		"\u0000\u0000\u0000FG\u0005{\u0000\u0000G\u000e\u0001\u0000\u0000\u0000"+
		"HI\u0005}\u0000\u0000I\u0010\u0001\u0000\u0000\u0000JK\u0005+\u0000\u0000"+
		"K\u0012\u0001\u0000\u0000\u0000LM\u0005;\u0000\u0000M\u0014\u0001\u0000"+
		"\u0000\u0000NO\u0005,\u0000\u0000O\u0016\u0001\u0000\u0000\u0000PQ\u0005"+
		"=\u0000\u0000Q\u0018\u0001\u0000\u0000\u0000RW\u0003\u001b\r\u0000SV\u0003"+
		"\u001b\r\u0000TV\u0003\u001f\u000f\u0000US\u0001\u0000\u0000\u0000UT\u0001"+
		"\u0000\u0000\u0000VY\u0001\u0000\u0000\u0000WU\u0001\u0000\u0000\u0000"+
		"WX\u0001\u0000\u0000\u0000X\u001a\u0001\u0000\u0000\u0000YW\u0001\u0000"+
		"\u0000\u0000Z[\u0003\u001d\u000e\u0000[\u001c\u0001\u0000\u0000\u0000"+
		"\\]\u0007\u0000\u0000\u0000]\u001e\u0001\u0000\u0000\u0000^_\u0007\u0001"+
		"\u0000\u0000_ \u0001\u0000\u0000\u0000`a\u0003#\u0011\u0000a\"\u0001\u0000"+
		"\u0000\u0000be\u0003%\u0012\u0000ce\u0003\'\u0013\u0000db\u0001\u0000"+
		"\u0000\u0000dc\u0001\u0000\u0000\u0000e$\u0001\u0000\u0000\u0000fj\u0003"+
		")\u0014\u0000gi\u0003\u001f\u000f\u0000hg\u0001\u0000\u0000\u0000il\u0001"+
		"\u0000\u0000\u0000jh\u0001\u0000\u0000\u0000jk\u0001\u0000\u0000\u0000"+
		"k&\u0001\u0000\u0000\u0000lj\u0001\u0000\u0000\u0000mq\u00050\u0000\u0000"+
		"np\u0003+\u0015\u0000on\u0001\u0000\u0000\u0000ps\u0001\u0000\u0000\u0000"+
		"qo\u0001\u0000\u0000\u0000qr\u0001\u0000\u0000\u0000r(\u0001\u0000\u0000"+
		"\u0000sq\u0001\u0000\u0000\u0000tu\u0007\u0002\u0000\u0000u*\u0001\u0000"+
		"\u0000\u0000vw\u0007\u0003\u0000\u0000w,\u0001\u0000\u0000\u0000x|\u0005"+
		"#\u0000\u0000y{\u0003/\u0017\u0000zy\u0001\u0000\u0000\u0000{~\u0001\u0000"+
		"\u0000\u0000|z\u0001\u0000\u0000\u0000|}\u0001\u0000\u0000\u0000}\u0082"+
		"\u0001\u0000\u0000\u0000~|\u0001\u0000\u0000\u0000\u007f\u0081\b\u0004"+
		"\u0000\u0000\u0080\u007f\u0001\u0000\u0000\u0000\u0081\u0084\u0001\u0000"+
		"\u0000\u0000\u0082\u0080\u0001\u0000\u0000\u0000\u0082\u0083\u0001\u0000"+
		"\u0000\u0000\u0083\u0085\u0001\u0000\u0000\u0000\u0084\u0082\u0001\u0000"+
		"\u0000\u0000\u0085\u0086\u0006\u0016\u0000\u0000\u0086.\u0001\u0000\u0000"+
		"\u0000\u0087\u0089\u0007\u0005\u0000\u0000\u0088\u0087\u0001\u0000\u0000"+
		"\u0000\u0089\u008a\u0001\u0000\u0000\u0000\u008a\u0088\u0001\u0000\u0000"+
		"\u0000\u008a\u008b\u0001\u0000\u0000\u0000\u008b\u008c\u0001\u0000\u0000"+
		"\u0000\u008c\u008d\u0006\u0017\u0000\u0000\u008d0\u0001\u0000\u0000\u0000"+
		"\u008e\u0090\u0005\r\u0000\u0000\u008f\u0091\u0005\n\u0000\u0000\u0090"+
		"\u008f\u0001\u0000\u0000\u0000\u0090\u0091\u0001\u0000\u0000\u0000\u0091"+
		"\u0094\u0001\u0000\u0000\u0000\u0092\u0094\u0005\n\u0000\u0000\u0093\u008e"+
		"\u0001\u0000\u0000\u0000\u0093\u0092\u0001\u0000\u0000\u0000\u0094\u0095"+
		"\u0001\u0000\u0000\u0000\u0095\u0096\u0006\u0018\u0000\u0000\u00962\u0001"+
		"\u0000\u0000\u0000\u000b\u0000UWdjq|\u0082\u008a\u0090\u0093\u0001\u0006"+
		"\u0000\u0000";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}