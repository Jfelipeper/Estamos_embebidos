/* vim:set filetype=typst: */
#import "@preview/fletcher:0.4.3" as fletcher: diagram, node, edge
#set rect(radius: 5pt)

#set text(10pt)

#diagram(

let (espera,a,b,ab) = ((0,0),(2,0),(1,1),(1,-1)),
  node-stroke: .1em,
  node-fill: gradient.radial(blue.lighten(80%), blue, center: (30%, 20%), radius: 80%),
  spacing: 7em,


  edge((-1,0),"r", "-|>", `Inicio`, label-pos: 0, label-side: center),

  node(espera, `Espera`, radius: 4em),
  node(a, `wait A`, radius: 2em, fill: gradient.radial(red.lighten(80%), red, center: (30%, 20%), radius: 80%)),
  node(b,`wait B`,radius: 2em, fill: gradient.radial(red.lighten(80%), red, center: (30%, 20%), radius: 80%)),
  node(ab, `wait AB`, radius: 2em,fill: gradient.radial(red.lighten(80%), red, center: (30%, 20%), radius: 80%)),


  edge(espera,a,`A=1`,"-|>",bend:12deg,label-side:center),
  edge(espera,ab, "-|>", text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","A & B=1"), bend:15deg, label-side: center,label-pos:0.15),
  edge(espera,b,`B=1`,"-|>",bend:-25deg,label-side:center),
  edge(espera, espera, rect(`t>60 -> Lectura`), "-|>-", bend: 110deg,label-pos:0.33),
  edge(espera, espera, `t<60`, "-|>-", bend:-110deg),

  edge(a,(2.3,0),(2.3,0.25),(2,0.25),a,`t<2`,"-|>",corner-radius:12pt,label-side:center),
  edge(a,b,`B=1 & A!=1`,"-|>",bend:30deg,label-pos:0.6,label-side:center),
  edge(a,ab,text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","A & B=1"),"-|>",bend:-30deg,label-pos:0.6,label-side:center),
  edge(a,espera,"-|>",rect(`t>2 -> UART`),bend:12deg,label-side:center),

  edge(b,b,`t<2`,"-|>-",bend: 110deg),
  edge(b,a,`A=1 & B!=1`,"-|>",bend:5deg,label-side:center,label-pos:0.6),
  edge(b,espera,rect(`t>2 -> Tabla`),"-|>",bend:-5deg,label-side:center),
  edge(b,ab,text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","A & B=1"),"-|>",bend:-100deg,label-side:center),

  edge(ab,ab,text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","(A & B=1)  &  t<5  ->  toggle  led"),"-|>-",bend: 110deg),
  edge(ab,a,`A=1 & B!=1`,"-|>",bend:-14deg,label-side:center,label-pos:0.6),
  edge(ab,espera,text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","A & B!=1"),"-|>",label-side:center,bend:8deg,label-pos:0.6),
  edge(ab,espera,rect(`t>5 -> Borra`),"-|>",label-side:center,bend:32deg,label-pos:0.24),
  edge(ab,b,`B=1 & A!=1`,bend:90deg,label-pos:0.4,label-side:center)
)



#diagram(

let (espera,a,b,ab) = ((0,0),(2,0),(1,1),(1,-1)),
  node-stroke: .1em,
  node-fill: gradient.radial(blue.lighten(80%), blue, center: (30%, 20%), radius: 80%),
  spacing: 7em,


  edge((-1,0),"r", "-|>", `Inicio`, label-pos: 0, label-side: center),

  node(espera, `Espera`, radius: 4em),
  node(a, `wait A`, radius: 2em, fill: gradient.radial(red.lighten(80%), red, center: (30%, 20%), radius: 80%)),
  node(b,`wait B`,radius: 2em, fill: gradient.radial(red.lighten(80%), red, center: (30%, 20%), radius: 80%)),
  node(ab, `wait AB`, radius: 2em,fill: gradient.radial(red.lighten(80%), red, center: (30%, 20%), radius: 80%)),


  edge(espera,a,`Push A`,"-|>",bend:12deg,label-side:center),
  edge(espera,ab, "-|>", text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","Push AB"), bend:15deg, label-side: center,label-pos:0.15),
  edge(espera,b,`Push B`,"-|>",bend:-25deg,label-side:center),
  edge(espera, espera, rect(`Timeout -> Lectura`), "-|>-", bend: 110deg,label-pos:0.33),
  edge(espera, espera, `Null`, "-|>-", bend:-110deg),

  edge(a,(2.3,0),(2.3,0.25),(2,0.25),a,`Default`,"-|>",corner-radius:12pt,label-side:center),
  edge(a,b,`Push B`,"-|>",bend:30deg,label-pos:0.6,label-side:center),
  edge(a,ab,text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","Push AB"),"-|>",bend:-30deg,label-pos:0.6,label-side:center),
  edge(a,espera,"-|>",rect(`Timeout -> UART`),bend:12deg,label-side:center),

  edge(b,b,`Default`,"-|>-",bend: 110deg),
  edge(b,a,`Push A`,"-|>",bend:5deg,label-side:center,label-pos:0.6),
  edge(b,espera,rect(`Timeout -> Tabla`),"-|>",bend:-12deg,label-side:center),
  edge(b,ab,text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","Push AB"),"-|>",bend:-100deg,label-side:center),

  edge(ab,ab,text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","Push AB  &  t<5  ->  toggle  led"),"-|>-",bend: 110deg),
  edge(ab,a,`Push A`,"-|>",bend:-14deg,label-side:center,label-pos:0.6),
  edge(ab,espera,text(size: 0.8em,spacing: 0.2em,font: "FiraCode Nerd Font","Null"),"-|>",label-side:center,bend:8deg,label-pos:0.6),
  edge(ab,espera,rect(`Timeout -> Borra`),"-|>",label-side:center,bend:32deg,label-pos:0.24),
  edge(ab,b,`Push B`,bend:93deg,label-pos:0.4,label-side:center)
)
