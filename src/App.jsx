import { useEffect, useState } from "react";

function App() {
  const [Module, setModule] = useState(null);
  const [input, setInput] = useState("");
  const [formatted, setFormatted] = useState("");
  const [postfix, setPostfix] = useState("");
  const [result, setResult] = useState("");
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const loadModule = async () => {
      try {
        // Import the module factory
        const moduleFactory = await import("./util/out/rpn.js");
        // Call the factory to create the module instance
        const mod = await moduleFactory.default();
        setModule(mod);
        setLoading(false);
        console.log("✅ RPN WebAssembly module loaded", mod);
      } catch (error) {
        console.error("❌ Failed to load WASM module:", error);
        setLoading(false);
      }
    };

    loadModule();
  }, []);

  const handleCalculate = () => {
    if (!Module) {
      alert("WASM module not ready yet!");
      return;
    }

    try {
      // Use cwrap for better performance (creates reusable function wrappers)
      const formatInfix = Module.cwrap('formatInfix', 'string', ['string']);
      const toPostfix = Module.cwrap('toPostfix', 'string', ['string']);
      const calculate = Module.cwrap('calculate', 'number', ['string']);

      // Call the functions
      const formattedExpr = formatInfix(input);
      const postfixExpr = toPostfix(formattedExpr);
      const calculationResult = calculate(input);

      setFormatted(formattedExpr);
      setPostfix(postfixExpr);
      setResult(calculationResult.toString());
    } catch (err) {
      console.error("Calculation error:", err);
      alert("Error calculating expression: " + err.message);
    }
  };

  const handleKeyPress = (e) => {
    if (e.key === 'Enter') {
      handleCalculate();
    }
  };

  if (loading) {
    return (
      <div className="flex flex-col items-center justify-center min-h-screen bg-gray-900 text-gray-100">
        <div className="text-xl">Loading WebAssembly module...</div>
        <div className="mt-4 text-sm text-gray-400">This may take a moment</div>
      </div>
    );
  }

  return (
    <div className="flex flex-col items-center justify-center min-h-screen bg-gray-900 text-gray-100 p-6">
      <h1 className="text-3xl font-bold mb-6">🧮 RPN Calculator</h1>

      <div className="w-full max-w-md bg-gray-800 rounded-2xl shadow-lg p-6 space-y-4">
        <input
          type="text"
          placeholder="Enter infix expression (e.g., 4 + 7 / 2)"
          className="w-full px-3 py-2 text-gray-900 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-400"
          value={input}
          onChange={(e) => setInput(e.target.value)}
          onKeyPress={handleKeyPress}
          disabled={!Module}
        />

        <button
          onClick={handleCalculate}
          disabled={!Module}
          className="w-full bg-blue-600 hover:bg-blue-700 disabled:bg-gray-600 rounded-lg py-2 font-semibold transition"
        >
          {Module ? "Calculate" : "Loading..."}
        </button>

        <div className="text-sm space-y-2">
          <div>
            <span className="font-semibold text-blue-400">Formatted Infix:</span>{" "}
            <span className="text-gray-200">{formatted || "—"}</span>
          </div>
          <div>
            <span className="font-semibold text-blue-400">Postfix:</span>{" "}
            <span className="text-gray-200">{postfix || "—"}</span>
          </div>
          <div>
            <span className="font-semibold text-blue-400">Result:</span>{" "}
            <span className="text-green-400">{result || "—"}</span>
          </div>
        </div>

        {!Module && (
          <div className="text-yellow-400 text-sm text-center">
            Module still loading... try again in a moment
          </div>
        )}
      </div>
    </div>
  );
}

export default App;