(window.webpackJsonp=window.webpackJsonp||[]).push([[18],{119:function(e,a,t){"use strict";t.r(a),t.d(a,"frontMatter",(function(){return c})),t.d(a,"metadata",(function(){return b})),t.d(a,"rightToc",(function(){return p})),t.d(a,"default",(function(){return i}));var n=t(1),m=t(6),s=(t(0),t(144)),c={id:"maximal_matching",title:"Maximal Matching"},b={id:"benchmarks/covering/maximal_matching",title:"Maximal Matching",description:"## Problem Specification",source:"@site/docs/benchmarks/covering/maximal_matching.md",permalink:"/gbbs/docs/benchmarks/covering/maximal_matching",editUrl:"https://github.com/facebook/docusaurus/edit/master/website/docs/benchmarks/covering/maximal_matching.md",sidebar:"docs",previous:{title:"Maximal Independent Set",permalink:"/gbbs/docs/benchmarks/covering/maximal_independent_set"},next:{title:"Graph Coloring",permalink:"/gbbs/docs/benchmarks/covering/graph_coloring"}},p=[{value:"Problem Specification",id:"problem-specification",children:[]},{value:"Algorithm Implementations",id:"algorithm-implementations",children:[]},{value:"Cost Bounds",id:"cost-bounds",children:[]},{value:"Compiling and Running",id:"compiling-and-running",children:[]}],r={rightToc:p};function i(e){var a=e.components,t=Object(m.a)(e,["components"]);return Object(s.b)("wrapper",Object(n.a)({},r,t,{components:a,mdxType:"MDXLayout"}),Object(s.b)("h2",{id:"problem-specification"},"Problem Specification"),Object(s.b)("h4",{id:"input"},"Input"),Object(s.b)("p",null,Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"G"),Object(s.b)("mo",{parentName:"mrow"},"="),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(s.b)("mi",{parentName:"mrow"},"V"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{separator:"true"}),","),Object(s.b)("mi",{parentName:"mrow"},"E"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"G=(V, E)")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.68333em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"G"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mrel"}),"="),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}}))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.22222em"}}),"V"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mpunct"}),","),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.16666666666666666em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.05764em"}}),"E"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")"))))),", an unweighted graph, and a source, ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"s"),Object(s.b)("mo",{parentName:"mrow"},"\u2208"),Object(s.b)("mi",{parentName:"mrow"},"V")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"s \\in V")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.5782em",verticalAlign:"-0.0391em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"s"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mrel"}),"\u2208"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}}))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.68333em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.22222em"}}),"V"))))),". The input\ngraph can either be undirected, or directed."),Object(s.b)("h4",{id:"output"},"Output"),Object(s.b)("p",null,Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"P")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"P")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.68333em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.13889em"}}),"P"))))),", a ",Object(s.b)("a",Object(n.a)({parentName:"p"},{href:"/benchmarks/definitions/"}),"mapping")," where ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"P"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"["),Object(s.b)("mi",{parentName:"mrow"},"v"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"]")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"P[v]")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.13889em"}}),"P"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"["),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.03588em"}}),"v"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),"]")))))," is the parent\nof ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"v")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"v")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.03588em"}}),"v")))))," in the output BFS-tree rooted at ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"s")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"s")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"s"))))),", and ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"P"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"["),Object(s.b)("mi",{parentName:"mrow"},"s"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"]"),Object(s.b)("mo",{parentName:"mrow"},"="),Object(s.b)("mi",{parentName:"mrow"},"s")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"P[s] = s")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.13889em"}}),"P"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"["),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"s"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),"]"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mrel"}),"="),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2777777777777778em"}}))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"0.43056em",verticalAlign:"0em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"s"))))),"."),Object(s.b)("h2",{id:"algorithm-implementations"},"Algorithm Implementations"),Object(s.b)("p",null,"We provide a single implementation of BFS in GBBS. The implementation\nis based on the non-deterministic BFS implementation in GBBS. We\nprovide a tutorial on how to implement this BFS example in ",Object(s.b)("a",Object(n.a)({parentName:"p"},{href:"tutorial/bfs_tutorial"}),"our\ntutorial"),"."),Object(s.b)("p",null,"The code for our implemenation is available\n",Object(s.b)("a",Object(n.a)({parentName:"p"},{href:"https://github.com/ldhulipala/gbbs/tree/master/benchmarks/BFS/NonDeterministicBFS"}),"here"),"."),Object(s.b)("h2",{id:"cost-bounds"},"Cost Bounds"),Object(s.b)("p",null,"The algorithm runs in ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"O"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(s.b)("mi",{parentName:"mrow"},"n"),Object(s.b)("mo",{parentName:"mrow"},"+"),Object(s.b)("mi",{parentName:"mrow"},"m"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"O(n + m)")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.02778em"}}),"O"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"n"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2222222222222222em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mbin"}),"+"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.2222222222222222em"}}))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"m"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")")))))," work and ",Object(s.b)("span",Object(n.a)({parentName:"p"},{className:"math math-inline"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-mathml"}),Object(s.b)("math",Object(n.a)({parentName:"span"},{xmlns:"http://www.w3.org/1998/Math/MathML"}),Object(s.b)("semantics",{parentName:"math"},Object(s.b)("mrow",{parentName:"semantics"},Object(s.b)("mi",{parentName:"mrow"},"O"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(s.b)("mi",Object(n.a)({parentName:"mrow"},{mathvariant:"sans-serif"}),"D"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),"("),Object(s.b)("mi",{parentName:"mrow"},"G"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")"),Object(s.b)("mi",{parentName:"mrow"},"log"),Object(s.b)("mo",{parentName:"mrow"},"\u2061"),Object(s.b)("mi",{parentName:"mrow"},"n"),Object(s.b)("mo",Object(n.a)({parentName:"mrow"},{stretchy:"false"}),")")),Object(s.b)("annotation",Object(n.a)({parentName:"semantics"},{encoding:"application/x-tex"}),"O(\\mathsf{D}(G) \\log n)")))),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"katex-html","aria-hidden":"true"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"base"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"strut",style:{height:"1em",verticalAlign:"-0.25em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault",style:{marginRight:"0.02778em"}}),"O"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord"}),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathsf"}),"D")),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mopen"}),"("),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"G"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.16666666666666666em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mop"}),"lo",Object(s.b)("span",Object(n.a)({parentName:"span"},{style:{marginRight:"0.01389em"}}),"g")),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mspace",style:{marginRight:"0.16666666666666666em"}})),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mord mathdefault"}),"n"),Object(s.b)("span",Object(n.a)({parentName:"span"},{className:"mclose"}),")"))))),"\ndepth, which follows from our bounds on the ",Object(s.b)("inlineCode",{parentName:"p"},"edgeMap")," primitive."),Object(s.b)("h2",{id:"compiling-and-running"},"Compiling and Running"),Object(s.b)("p",null,"The benchmark can be compiled by running:"),Object(s.b)("pre",null,Object(s.b)("code",Object(n.a)({parentName:"pre"},{}),"bazel build -c opt //benchmarks/BFS/NonDeterministicBFS:BFS\n")),Object(s.b)("p",null,"It can then be run on a test input graph in the ",Object(s.b)("em",{parentName:"p"},"uncompressed format")," as follows:"),Object(s.b)("pre",null,Object(s.b)("code",Object(n.a)({parentName:"pre"},{}),"numactl -i all ./bazel-bin/benchmarks/BFS/NonDeterministicBFS/BFS_main -s -m -src 1 inputs/rMatGraph_J_5_100\n")),Object(s.b)("p",null,"It can then be run on a test input graph in the ",Object(s.b)("em",{parentName:"p"},"compressed format")," as follows:"),Object(s.b)("pre",null,Object(s.b)("code",Object(n.a)({parentName:"pre"},{}),"numactl -i all ./bazel-bin/benchmarks/BFS/NonDeterministicBFS/BFS_main -s -c -m -src 1 inputs/rMatGraph_J_5_100.bytepda\n")))}i.isMDXComponent=!0},144:function(e,a,t){"use strict";t.d(a,"a",(function(){return l})),t.d(a,"b",(function(){return o}));var n=t(0),m=t.n(n);function s(e,a,t){return a in e?Object.defineProperty(e,a,{value:t,enumerable:!0,configurable:!0,writable:!0}):e[a]=t,e}function c(e,a){var t=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);a&&(n=n.filter((function(a){return Object.getOwnPropertyDescriptor(e,a).enumerable}))),t.push.apply(t,n)}return t}function b(e){for(var a=1;a<arguments.length;a++){var t=null!=arguments[a]?arguments[a]:{};a%2?c(Object(t),!0).forEach((function(a){s(e,a,t[a])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(t)):c(Object(t)).forEach((function(a){Object.defineProperty(e,a,Object.getOwnPropertyDescriptor(t,a))}))}return e}function p(e,a){if(null==e)return{};var t,n,m=function(e,a){if(null==e)return{};var t,n,m={},s=Object.keys(e);for(n=0;n<s.length;n++)t=s[n],a.indexOf(t)>=0||(m[t]=e[t]);return m}(e,a);if(Object.getOwnPropertySymbols){var s=Object.getOwnPropertySymbols(e);for(n=0;n<s.length;n++)t=s[n],a.indexOf(t)>=0||Object.prototype.propertyIsEnumerable.call(e,t)&&(m[t]=e[t])}return m}var r=m.a.createContext({}),i=function(e){var a=m.a.useContext(r),t=a;return e&&(t="function"==typeof e?e(a):b({},a,{},e)),t},l=function(e){var a=i(e.components);return m.a.createElement(r.Provider,{value:a},e.children)},O={inlineCode:"code",wrapper:function(e){var a=e.children;return m.a.createElement(m.a.Fragment,{},a)}},j=Object(n.forwardRef)((function(e,a){var t=e.components,n=e.mdxType,s=e.originalType,c=e.parentName,r=p(e,["components","mdxType","originalType","parentName"]),l=i(t),j=n,o=l["".concat(c,".").concat(j)]||l[j]||O[j]||s;return t?m.a.createElement(o,b({ref:a},r,{components:t})):m.a.createElement(o,b({ref:a},r))}));function o(e,a){var t=arguments,n=a&&a.mdxType;if("string"==typeof e||n){var s=t.length,c=new Array(s);c[0]=j;var b={};for(var p in a)hasOwnProperty.call(a,p)&&(b[p]=a[p]);b.originalType=e,b.mdxType="string"==typeof e?e:n,c[1]=b;for(var r=2;r<s;r++)c[r]=t[r];return m.a.createElement.apply(null,c)}return m.a.createElement.apply(null,t)}j.displayName="MDXCreateElement"}}]);