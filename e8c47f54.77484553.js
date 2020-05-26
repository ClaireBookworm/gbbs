(window.webpackJsonp=window.webpackJsonp||[]).push([[35],{137:function(e,a,t){"use strict";t.r(a),t.d(a,"frontMatter",(function(){return c})),t.d(a,"metadata",(function(){return b})),t.d(a,"rightToc",(function(){return p})),t.d(a,"default",(function(){return i}));var n=t(1),s=t(6),m=(t(0),t(144)),c={id:"breadth_first_search",title:"BFS"},b={id:"benchmarks/sssp/breadth_first_search",title:"BFS",description:"## Problem Specification",source:"@site/docs/benchmarks/sssp/breadth_first_search.md",permalink:"/gbbs/docs/benchmarks/sssp/breadth_first_search",editUrl:"https://github.com/facebook/docusaurus/edit/master/website/docs/benchmarks/sssp/breadth_first_search.md",sidebar:"docs",previous:{title:"Definitions",permalink:"/gbbs/docs/benchmarks/definitions"},next:{title:"Integral-Weight SSSP (weighted BFS)",permalink:"/gbbs/docs/benchmarks/sssp/integral_weight_sssp"}},p=[{value:"Problem Specification",id:"problem-specification",children:[]},{value:"Algorithm Implementations",id:"algorithm-implementations",children:[]},{value:"Cost Bounds",id:"cost-bounds",children:[]},{value:"Compiling and Running",id:"compiling-and-running",children:[]}],r={rightToc:p};function i(e){var a=e.components,t=Object(s.a)(e,["components"]);return Object(m.b)("wrapper",Object(n.a)({},r,t,{components:a,mdxType:"MDXLayout"}),Object(m.b)("h2",{id:"problem-specification"},"Problem Specification"),Object(m.b)("h4",{id:"input"},"Input"),Object(m.b)("p",null,Object(m.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(m.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(m.b)("semantics",{parentName:"math"},Object(m.b)("mrow",{parentName:"semantics"},Object(m.b)("mi",{parentName:"mrow"},"G"),Object(m.b)("mo",{parentName:"mrow"},"="),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(m.b)("mi",{parentName:"mrow"},"V"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{separator:"true"}),","),Object(m.b)("mi",{parentName:"mrow"},"E"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(m.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"G=(V, E)")))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.68333em",verticalAlign:"0em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"G"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mrel"}),"="),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}}))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.22222em"}}),"V"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mpunct"}),","),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.16666666666666666em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.05764em"}}),"E"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")"))))),", an unweighted graph, and a source, ",Object(m.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(m.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(m.b)("semantics",{parentName:"math"},Object(m.b)("mrow",{parentName:"semantics"},Object(m.b)("mi",{parentName:"mrow"},"s"),Object(m.b)("mo",{parentName:"mrow"},"\u2208"),Object(m.b)("mi",{parentName:"mrow"},"V")),Object(m.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"s \\in V")))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.5782em",verticalAlign:"-0.0391em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"s"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mrel"}),"\u2208"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}}))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.68333em",verticalAlign:"0em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.22222em"}}),"V"))))),". The input\ngraph can either be undirected, or directed."),Object(m.b)("h4",{id:"output"},"Output"),Object(m.b)("p",null,Object(m.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(m.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(m.b)("semantics",{parentName:"math"},Object(m.b)("mrow",{parentName:"semantics"},Object(m.b)("mi",{parentName:"mrow"},"P")),Object(m.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"P")))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.68333em",verticalAlign:"0em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.13889em"}}),"P"))))),", a ",Object(m.b)("a",Object(n.a)({parentName:"p"},{href:"/benchmarks/definitions/"}),"mapping")," where ",Object(m.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(m.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(m.b)("semantics",{parentName:"math"},Object(m.b)("mrow",{parentName:"semantics"},Object(m.b)("mi",{parentName:"mrow"},"P"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"["),Object(m.b)("mi",{parentName:"mrow"},"v"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"]")),Object(m.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"P[v]")))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.13889em"}}),"P"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"["),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.03588em"}}),"v"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),"]")))))," is the parent\nof ",Object(m.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(m.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(m.b)("semantics",{parentName:"math"},Object(m.b)("mrow",{parentName:"semantics"},Object(m.b)("mi",{parentName:"mrow"},"v")),Object(m.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"v")))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.03588em"}}),"v")))))," in the output BFS-tree rooted at ",Object(m.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(m.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(m.b)("semantics",{parentName:"math"},Object(m.b)("mrow",{parentName:"semantics"},Object(m.b)("mi",{parentName:"mrow"},"s")),Object(m.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"s")))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"s"))))),", and ",Object(m.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(m.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(m.b)("semantics",{parentName:"math"},Object(m.b)("mrow",{parentName:"semantics"},Object(m.b)("mi",{parentName:"mrow"},"P"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"["),Object(m.b)("mi",{parentName:"mrow"},"s"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"]"),Object(m.b)("mo",{parentName:"mrow"},"="),Object(m.b)("mi",{parentName:"mrow"},"s")),Object(m.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"P[s] = s")))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.13889em"}}),"P"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"["),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"s"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),"]"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mrel"}),"="),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}}))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"s"))))),"."),Object(m.b)("h2",{id:"algorithm-implementations"},"Algorithm Implementations"),Object(m.b)("p",null,"We provide a single implementation of BFS in GBBS. The implementation\nis based on the non-deterministic BFS implementation in GBBS. We\nprovide a tutorial on how to implement this BFS example in ",Object(m.b)("a",Object(n.a)({parentName:"p"},{href:"tutorial/bfs_tutorial"}),"our\ntutorial"),"."),Object(m.b)("p",null,"The code for our implemenation is available\n",Object(m.b)("a",Object(n.a)({parentName:"p"},{href:"https://github.com/ldhulipala/gbbs/tree/master/benchmarks/BFS/NonDeterministicBFS"}),"here"),"."),Object(m.b)("h2",{id:"cost-bounds"},"Cost Bounds"),Object(m.b)("p",null,"The algorithm runs in ",Object(m.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(m.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(m.b)("semantics",{parentName:"math"},Object(m.b)("mrow",{parentName:"semantics"},Object(m.b)("mi",{parentName:"mrow"},"O"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(m.b)("mi",{parentName:"mrow"},"n"),Object(m.b)("mo",{parentName:"mrow"},"+"),Object(m.b)("mi",{parentName:"mrow"},"m"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(m.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"O(n + m)")))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.02778em"}}),"O"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"n"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2222222222222222em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mbin"}),"+"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2222222222222222em"}}))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"m"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")")))))," work and ",Object(m.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(m.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(m.b)("semantics",{parentName:"math"},Object(m.b)("mrow",{parentName:"semantics"},Object(m.b)("mi",{parentName:"mrow"},"O"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(m.b)("mi",Object(n.a)({parentName:"mrow"},{mathvariant:"sans-serif"}),"D"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(m.b)("mi",{parentName:"mrow"},"G"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")"),Object(m.b)("mi",{parentName:"mrow"},"log"),Object(m.b)("mo",{parentName:"mrow"},"\u2061"),Object(m.b)("mi",{parentName:"mrow"},"n"),Object(m.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(m.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"O(\\mathsf{D}(G) \\log n)")))),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.02778em"}}),"O"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord"}),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathsf"}),"D")),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"G"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.16666666666666666em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mop"}),"lo",Object(m.b)("span",Object(n.a)({parentName:"span"},{style:{marginRight:"0.01389em"}}),"g")),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.16666666666666666em"}})),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"n"),Object(m.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")"))))),"\ndepth, which follows from our bounds on the ",Object(m.b)("inlineCode",{parentName:"p"},"edgeMap")," primitive."),Object(m.b)("h2",{id:"compiling-and-running"},"Compiling and Running"),Object(m.b)("p",null,"The benchmark can be compiled by running:"),Object(m.b)("pre",null,Object(m.b)("code",Object(n.a)({parentName:"pre"},{}),"bazel build -c opt //benchmarks/BFS/NonDeterministicBFS:BFS\n")),Object(m.b)("p",null,"It can then be run on a test input graph in the ",Object(m.b)("em",{parentName:"p"},"uncompressed format")," as follows:"),Object(m.b)("pre",null,Object(m.b)("code",Object(n.a)({parentName:"pre"},{}),"numactl -i all ./bazel-bin/benchmarks/BFS/NonDeterministicBFS/BFS_main -s -m -src 1 inputs/rMatGraph_J_5_100\n")),Object(m.b)("p",null,"It can then be run on a test input graph in the ",Object(m.b)("em",{parentName:"p"},"compressed format")," as follows:"),Object(m.b)("pre",null,Object(m.b)("code",Object(n.a)({parentName:"pre"},{}),"numactl -i all ./bazel-bin/benchmarks/BFS/NonDeterministicBFS/BFS_main -s -c -m -src 1 inputs/rMatGraph_J_5_100.bytepda\n")))}i.isMDXComponent=!0},144:function(e,a,t){"use strict";t.d(a,"a",(function(){return O})),t.d(a,"b",(function(){return o}));var n=t(0),s=t.n(n);function m(e,a,t){return a in e?Object.defineProperty(e,a,{value:t,enumerable:!0,configurable:!0,writable:!0}):e[a]=t,e}function c(e,a){var t=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);a&&(n=n.filter((function(a){return Object.getOwnPropertyDescriptor(e,a).enumerable}))),t.push.apply(t,n)}return t}function b(e){for(var a=1;a<arguments.length;a++){var t=null!=arguments[a]?arguments[a]:{};a%2?c(Object(t),!0).forEach((function(a){m(e,a,t[a])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(t)):c(Object(t)).forEach((function(a){Object.defineProperty(e,a,Object.getOwnPropertyDescriptor(t,a))}))}return e}function p(e,a){if(null==e)return{};var t,n,s=function(e,a){if(null==e)return{};var t,n,s={},m=Object.keys(e);for(n=0;n<m.length;n++)t=m[n],a.indexOf(t)>=0||(s[t]=e[t]);return s}(e,a);if(Object.getOwnPropertySymbols){var m=Object.getOwnPropertySymbols(e);for(n=0;n<m.length;n++)t=m[n],a.indexOf(t)>=0||Object.prototype.propertyIsEnumerable.call(e,t)&&(s[t]=e[t])}return s}var r=s.a.createContext({}),i=function(e){var a=s.a.useContext(r),t=a;return e&&(t="function"==typeof e?e(a):b({},a,{},e)),t},O=function(e){var a=i(e.components);return s.a.createElement(r.Provider,{value:a},e.children)},l={inlineCode:"code",wrapper:function(e){var a=e.children;return s.a.createElement(s.a.Fragment,{},a)}},j=Object(n.forwardRef)((function(e,a){var t=e.components,n=e.mdxType,m=e.originalType,c=e.parentName,r=p(e,["components","mdxType","originalType","parentName"]),O=i(t),j=n,o=O["".concat(c,".").concat(j)]||O[j]||l[j]||m;return t?s.a.createElement(o,b({ref:a},r,{components:t})):s.a.createElement(o,b({ref:a},r))}));function o(e,a){var t=arguments,n=a&&a.mdxType;if("string"==typeof e||n){var m=t.length,c=new Array(m);c[0]=j;var b={};for(var p in a)hasOwnProperty.call(a,p)&&(b[p]=a[p]);b.originalType=e,b.mdxType="string"==typeof e?e:n,c[1]=b;for(var r=2;r<m;r++)c[r]=t[r];return s.a.createElement.apply(null,c)}return s.a.createElement.apply(null,t)}j.displayName="MDXCreateElement"}}]);