(window.webpackJsonp=window.webpackJsonp||[]).push([[36],{138:function(e,t,n){"use strict";n.r(t),n.d(t,"frontMatter",(function(){return c})),n.d(t,"metadata",(function(){return i})),n.d(t,"rightToc",(function(){return s})),n.d(t,"default",(function(){return l}));var r=n(1),a=n(6),o=(n(0),n(147)),c={id:"formats",title:"Graph Formats"},i={id:"formats",title:"Graph Formats",description:"We support the adjacency graph format used by the [Problem Based Benchmark",source:"@site/docs/formats.md",permalink:"/gbbs/docs/formats",sidebar:"docs",previous:{title:"Running Benchmarks",permalink:"/gbbs/docs/run"},next:{title:"Python Bindings",permalink:"/gbbs/docs/python_bindings"}},s=[],p={rightToc:s};function l(e){var t=e.components,n=Object(a.a)(e,["components"]);return Object(o.b)("wrapper",Object(r.a)({},p,n,{components:t,mdxType:"MDXLayout"}),Object(o.b)("p",null,"We support the adjacency graph format used by the ",Object(o.b)("a",Object(r.a)({parentName:"p"},{href:"http://www.cs.cmu.edu/~pbbs/benchmarks/graphIO.html"}),"Problem Based Benchmark\nsuite"),"\nand ",Object(o.b)("a",Object(r.a)({parentName:"p"},{href:"https://github.com/jshun/ligra"}),"Ligra"),"."),Object(o.b)("p",null,"The adjacency graph format starts with a sequence of offsets one for each\nvertex, followed by a sequence of directed edges ordered by their source vertex.\nThe offset for a vertex i refers to the location of the start of a contiguous\nblock of out edges for vertex i in the sequence of edges. The block continues\nuntil the offset of the next vertex, or the end if i is the last vertex. All\nvertices and offsets are 0 based and represented in decimal. The specific format\nis as follows:"),Object(o.b)("pre",null,Object(o.b)("code",Object(r.a)({parentName:"pre"},{}),"AdjacencyGraph\n<n>\n<m>\n<o0>\n<o1>\n...\n<o(n-1)>\n<e0>\n<e1>\n...\n<e(m-1)>\n")),Object(o.b)("p",null,"This file is represented as plain text."),Object(o.b)("p",null,'Weighted graphs are represented in the weighted adjacency graph format. The file\nshould start with the string "WeightedAdjacencyGraph". The m edge weights\nshould be stored after all of the edge targets in the .adj file.'),Object(o.b)("p",null,Object(o.b)("strong",{parentName:"p"},"Using SNAP graphs")),Object(o.b)("p",null,"Graphs from the ",Object(o.b)("a",Object(r.a)({parentName:"p"},{href:"https://snap.stanford.edu/data/index.html"}),"SNAP dataset\ncollection")," are commonly used for\ngraph algorithm benchmarks. We provide a tool that converts the most common SNAP\ngraph format to the adjacency graph format that GBBS accepts. Usage example:"),Object(o.b)("pre",null,Object(o.b)("code",Object(r.a)({parentName:"pre"},{className:"language-sh"}),"# Download a graph from the SNAP collection.\nwget https://snap.stanford.edu/data/wiki-Vote.txt.gz\ngzip --decompress ${PWD}/wiki-Vote.txt.gz\n# Run the SNAP-to-adjacency-graph converter.\n# Run with Bazel:\nbazel run //utils:snap_converter -- -s -i ${PWD}/wiki-Vote.txt -o <output file>\n# Or run with Make:\n#   cd utils\n#   make snap_converter\n#   ./snap_converter -s -i <input file> -o <output file>\n")))}l.isMDXComponent=!0},147:function(e,t,n){"use strict";n.d(t,"a",(function(){return u})),n.d(t,"b",(function(){return h}));var r=n(0),a=n.n(r);function o(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function c(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,r)}return n}function i(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?c(Object(n),!0).forEach((function(t){o(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):c(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function s(e,t){if(null==e)return{};var n,r,a=function(e,t){if(null==e)return{};var n,r,a={},o=Object.keys(e);for(r=0;r<o.length;r++)n=o[r],t.indexOf(n)>=0||(a[n]=e[n]);return a}(e,t);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);for(r=0;r<o.length;r++)n=o[r],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(a[n]=e[n])}return a}var p=a.a.createContext({}),l=function(e){var t=a.a.useContext(p),n=t;return e&&(n="function"==typeof e?e(t):i({},t,{},e)),n},u=function(e){var t=l(e.components);return a.a.createElement(p.Provider,{value:t},e.children)},f={inlineCode:"code",wrapper:function(e){var t=e.children;return a.a.createElement(a.a.Fragment,{},t)}},d=Object(r.forwardRef)((function(e,t){var n=e.components,r=e.mdxType,o=e.originalType,c=e.parentName,p=s(e,["components","mdxType","originalType","parentName"]),u=l(n),d=r,h=u["".concat(c,".").concat(d)]||u[d]||f[d]||o;return n?a.a.createElement(h,i({ref:t},p,{components:n})):a.a.createElement(h,i({ref:t},p))}));function h(e,t){var n=arguments,r=t&&t.mdxType;if("string"==typeof e||r){var o=n.length,c=new Array(o);c[0]=d;var i={};for(var s in t)hasOwnProperty.call(t,s)&&(i[s]=t[s]);i.originalType=e,i.mdxType="string"==typeof e?e:r,c[1]=i;for(var p=2;p<o;p++)c[p]=n[p];return a.a.createElement.apply(null,c)}return a.a.createElement.apply(null,n)}d.displayName="MDXCreateElement"}}]);