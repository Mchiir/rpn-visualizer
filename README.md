# RPN Calculator with WebAssembly

A React-based Reverse Polish Notation (RPN) calculator that uses C++ compiled to WebAssembly for high-performance mathematical expression evaluation.

## Features

- **Infix to Postfix Conversion**: Convert standard mathematical expressions to RPN format
- **WebAssembly Powered**: C++ core compiled to WASM for optimal performance
- **Real-time Calculation**: Instant expression evaluation
- **Modern UI**: Clean, responsive React interface with Tailwind CSS
- **Cross-platform**: Works in any modern web browser

## Prerequisites

- **Windows**: Chocolatey package manager
- **Emscripten SDK**: For compiling C++ to WebAssembly
- **Node.js**: Version 16 or higher
- **npm** or **yarn**: Package manager

## Installation

### 1. Install Emscripten

#### Windows (using Chocolatey):

```bash
# Run as Administrator
choco install emscripten
```

#### Alternative Emscripten Installation:

```bash
# Clone and setup Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
emsdk install latest
emsdk activate latest
emsdk_env.bat
```

### 2. Verify Emscripten Installation

```bash
em++ -v
```

### 3. Compile C++ to WebAssembly

```bash
# Navigate to your project directory and run:
em++ src/util/rpn-calc.cpp -O3 -o src/util/out/rpn.js -s EXPORT_NAME=createRPNModule -s MODULARIZE=1 -s EXPORT_ES6=1 -s USE_ES6_IMPORT_META=0 -s "EXPORTED_FUNCTIONS=['_calculate','_formatInfix','_toPostfix','_evaluatePostfix']" -s "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
```

### 4. Install Dependencies

```bash
npm install
```

### 5. Start Development Server

```bash
npm run dev
```

## Project Structure

```
rpn-calc/
├── src/
│   ├── util/
│   │   ├── rpn-calc.cpp    # C++ RPN implementation
│   │   └── out/
            |__ rpn.js          # Compiled WebAssembly module
│   ├── App.jsx             # Main React component
│   └── main.jsx            # React entry point
├── index.html
├── package.json
└── vite.config.js
```

## Build Commands

### Development

```bash
npm run dev
```

### Production Build

```bash
npm run build
```

### Preview Production Build

```bash
npm run preview
```

## Available Functions

The WebAssembly module exposes the following C++ functions:

- `calculate(expression)`: Directly evaluate infix expression
- `formatInfix(expression)`: Format and validate infix expression
- `toPostfix(expression)`: Convert infix to postfix notation
- `evaluatePostfix(expression)`: Evaluate postfix expression

## Usage Examples

Enter expressions in standard mathematical notation:

- `4 + 7 / 2`
- `(3 + 4) * 2`
- `10 - 3 * 2 + 1`

The calculator will display:

- **Formatted Infix**: Properly spaced expression
- **Postfix**: RPN notation
- **Result**: Calculated value

## Recompiling C++ Code

When you modify `rpn-calc.cpp`, recompile with:

```bash
em++ src/util/rpn-calc.cpp -O3 -o src/util/rpn.js -s EXPORT_NAME=createRPNModule -s MODULARIZE=1 -s EXPORT_ES6=1 -s USE_ES6_IMPORT_META=0 -s "EXPORTED_FUNCTIONS=['_calculate','_formatInfix','_toPostfix','_evaluatePostfix']" -s "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
```

## Emscripten Flags Explained

- `-O3`: Maximum optimization
- `-s EXPORT_NAME=createRPNModule`: Names the module factory
- `-s MODULARIZE=1`: Enables module pattern
- `-s EXPORT_ES6=1`: Generates ES6 module format
- `-s USE_ES6_IMPORT_META=0`: Compatibility with build tools
- `EXPORTED_FUNCTIONS`: C++ functions exposed to JavaScript
- `EXPORTED_RUNTIME_METHODS`: Emscripten helper functions

## Troubleshooting

### Common Issues:

1. **"Module not found" errors**

   - Verify Emscripten installation
   - Check file paths in compilation command

2. **WebAssembly loading failures**

   - Ensure `rpn.js` and `rpn.wasm` are in correct location
   - Check browser console for specific errors

3. **Compilation errors**
   - Run `em++ -v` to verify Emscripten installation
   - Check C++ code for syntax errors

## Browser Compatibility

- Chrome 57+
- Firefox 52+
- Safari 11+
- Edge 16+

## License

MIT License - feel free to use this project for learning and development.

## Contributing

1. Fork the project
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Open a Pull Request

---

**Happy Calculating!**
