const CopyPlugin = require('copy-webpack-plugin');

module.exports = {
    context: __dirname,
    devtool: "source-map",
    mode: "development",
    entry: "./dev/renderer.js",
    output: {
      path: __dirname + "/dist",
      filename: "renderer.js"
    },
    plugins: [
      new CopyPlugin([
          {from:'build/Release/compressor.node', to:''}]), 
    ]
  }