// define ConceptMap object
var ConceptMap = function(svg) {
  var thisGraph = this;
  
  thisGraph.nodes = null;
  thisGraph.edges = null;
  
  thisGraph.state = {
    selectedNode: null,
    selectedEdge: null,
    mouseDownNode: null,
    mouseDownEdge: null,
    justDragged: false,
    justScaleTransGraph: false,
    lastKeyDown: -1,
    shiftRectDrag: false
  };

  // define arrow markers for graph links
  var defs = svg.append("svg:defs");
  defs.append("svg:marker")
    .attr("id", "end-arrow")
    .attr("viewBox", "0 -5 10 10")
    .attr("refX", "60")
    .attr("markerWidth", 5)
    .attr("markerHeight", 5)
    .attr("orient", "auto")
    .append("svg:path")
    .attr("d", "M0,-5L10,0L0,5");

  // define arrow markers for leading arrow
  defs.append("svg:marker")
    .attr("id", "mark-end-arrow")
    .attr("viewBox", "0 -5 10 10")
    .attr("refX", "0.1")
    .attr("markerWidth", 5)
    .attr("markerHeight", 5)
    .attr("orient", "auto")
    .append("svg:path")
    .attr("d", "M0,-5L10,0L0,5");

  thisGraph.svg = svg;
  thisGraph.svgG = svg.append("g")
        .classed(thisGraph.consts.graphClass, true);
  var svgG = thisGraph.svgG;

  // displayed when dragging between nodes
  thisGraph.dragLine = svgG.append("svg:path")
        .attr("class", "link dragline hidden")
        .attr("d", "M0,0L0,0")
        .style("marker-end", "url(#mark-end-arrow)");

  // svg nodes and edges 
  thisGraph.paths = svgG.append("g").selectAll("g");
  thisGraph.rects = svgG.append("g").selectAll("g");

  thisGraph.drag = d3.behavior.drag()
    .origin(function(d) {
      return {x: d.x, y: d.y};
    })
    .on("drag", function(args) {
      thisGraph.state.justDragged = true;
      thisGraph.dragmove.call(thisGraph, args);
    })
    .on("dragend", function() {
      // todo check if edge-mode is selected
    });

  // listen for key events
  d3.select(window).on("keydown", function() {
    thisGraph.svgKeyDown.call(thisGraph);
  })
  .on("keyup", function() {
    thisGraph.svgKeyUp.call(thisGraph);
  });
  
  svg.on("mousedown", function(d) {
    thisGraph.svgMouseDown.call(thisGraph, d);
  });
  svg.on("mouseup", function(d) {
    thisGraph.svgMouseUp.call(thisGraph, d);
  });

  // listen for dragging
  var dragSvg = d3.behavior.zoom()
    .on("zoom", function() {
      if (d3.event.sourceEvent.shiftKey) {
        // TODO  the internal d3 state is still changing
        return false;
      }
      else {
        thisGraph.zoomed.call(thisGraph);
      }
      return true;
    })
    .on("zoomstart", function() {
      var ael = d3.select("#" + thisGraph.consts.activeEditId).node();
      if (ael) {
        ael.blur();
      }
      if (!d3.event.sourceEvent.shiftKey) d3.select("body").style("cursor", "move");
    })
    .on("zoomend", function() {
      d3.select("body").style("cursor", "auto");
    });
  
  svg.call(dragSvg).on("dblclick.zoom", null);
};

ConceptMap.prototype.consts = {
  selectedClass: "selected",
  connectClass: "connect-node",
  rectGClass: "conceptG",
  graphClass: "graph",
  activeEditId: "active-editing",
  BACKSPACE_KEY: 8,
  DELETE_KEY: 46,
  ENTER_KEY: 13,
  rectWidth: 100,
  rectHeight: 50
};

/* PROTOTYPE FUNCTIONS */
ConceptMap.prototype.getNewId = function() {
  var nodes = this.nodes,
      edges = this.edges;
  
  var idArray = [];
  nodes.forEach(function(e) {
      idArray.push(e.id);
  });
  edges.forEach(function(e) {
      idArray.push(e.id);
  });
  
  idArray.sort(function(a,b) {
      return a-b;
  });
  if (idArray.length != 0) {
      var i = 0;
      for (i = 0; i < idArray.length; i++) {
          if (i != idArray[i]) {
              return i;
          }
      }
      return i;
  }
  else {
      return 0;
  }
};

ConceptMap.prototype.setData = function(nodes, edges) {
  var thisGraph = this;
  
  thisGraph.nodes = nodes;
  thisGraph.edges = edges;
};

ConceptMap.prototype.dragmove = function(d) {
  var thisGraph = this;
  
  // if shift key is being pressed on a rect, display dragLine
  if (thisGraph.state.shiftRectDrag) {
    thisGraph.dragLine.attr("d", "M" + d.x + "," + d.y + "L" + d3.mouse(thisGraph.svgG.node())[0] + "," + d3.mouse(this.svgG.node())[1]);
  }
  else {
    d.x += d3.event.dx;
    d.y +=  d3.event.dy;
    thisGraph.updateGraph();
  }
};

// select all text in element: taken from http://stackoverflow.com/questions/6139107/programatically-select-text-in-a-contenteditable-html-element
ConceptMap.prototype.selectElementText = function(element) {
  var range = document.createRange();
  range.selectNodeContents(element);
  var sel = window.getSelection();
  sel.removeAllRanges();
  sel.addRange(range);
};

// insert svg text: taken from http://stackoverflow.com/questions/13241475/how-do-i-include-newlines-in-labels-in-d3-charts
ConceptMap.prototype.insertNameToRect = function(gRect, d) {
  gRect.append("text")
    .attr("text-anchor","middle")
    .append("tspan").text(d.name);
};

ConceptMap.prototype.insertNameToPath = function(gPath, d) {
  var thisGraph = this,
      nodes = thisGraph.nodes;
      
  gPath.append("text")
    .attr("text-anchor","middle")
    //.attr("x", String((d.source.x + d.target.x)/2))
    .attr("x", function(d) {
      var n = nodes.filter(function(node) {
          if (node.id === d.source || node.id === d.target) {
              return node;
          }
      });
      return String(n[0].x + n[1].x)/2;
    })
    //.attr("y", String((d.source.y + d.target.y)/2))
    .attr("y", function(d) {
      var n = nodes.filter(function(node) {
          if (node.id === d.source || node.id === d.target) {
              return node;
          }
      });
      return String(n[0].y + n[1].y)/2;
    })
    .append("tspan").text(d.name)
};

// remove edges associated with a node
ConceptMap.prototype.spliceEdgesForNode = function(node) {
  var thisGraph = this,
      toSplice = thisGraph.edges.filter(function(e) {
    return (e.source === node.id || e.target === node.id);
  });
  toSplice.map(function(e) {
    thisGraph.edges.splice(thisGraph.edges.indexOf(e), 1);
  });
};

//
ConceptMap.prototype.replaceSelectEdge = function(d3Path, edgeData) {
  var thisGraph = this;
  d3Path.classed(thisGraph.consts.selectedClass, true);
  if (thisGraph.state.selectedEdge) {
    thisGraph.removeSelectFromEdge();
  }
  thisGraph.state.selectedEdge = edgeData;
};

ConceptMap.prototype.replaceSelectNode = function(d3Node, nodeData) {
  var thisGraph = this;
  d3Node.classed(thisGraph.consts.selectedClass, true);
  if (thisGraph.state.selectedNode) {
    thisGraph.removeSelectFromNode();
  }
  thisGraph.state.selectedNode = nodeData;
};

//
ConceptMap.prototype.removeSelectFromNode = function() {
  var thisGraph = this;
  thisGraph.rects.filter(function(rect) {
    return rect.id === thisGraph.state.selectedNode.id;
  }).classed(thisGraph.consts.selectedClass, false);
  thisGraph.state.selectedNode = null;
};

ConceptMap.prototype.removeSelectFromEdge = function() {
  var thisGraph = this;
  thisGraph.paths.filter(function(path) {
    return path.id === thisGraph.state.selectedEdge.id;
  }).classed(thisGraph.consts.selectedClass, false);
  thisGraph.state.selectedEdge = null;
};

//
ConceptMap.prototype.rectMouseDown = function(gRect, d) {
  var thisGraph = this,
      state = thisGraph.state;
  d3.event.stopPropagation();
  state.mouseDownNode = d;
  if (d3.event.shiftKey) {
    state.shiftRectDrag = d3.event.shiftKey;
    // reposition dragged directed edge
    thisGraph.dragLine.classed("hidden", false)
      .attr("d", "M" + d.x + "," + d.y + "L" + d.x + "," + d.y);
    return;
  }
};

// place editable text on rect in place of svg text
ConceptMap.prototype.changeTextOfNode = function(gRect, d) {
  var thisGraph = this,
      consts = thisGraph.consts,
      htmlEl = gRect.node();
  
  // remove all text element of current rect
  gRect.selectAll("text").remove();
  
  // get rect BCR to calculate the position of new editable element
  var rectBCR = htmlEl.getBoundingClientRect();
  var svgBCR = thisGraph.svg.node().getBoundingClientRect();
  
  // replace with editable content text
  var d3txt = thisGraph.svg.selectAll("foreignObject")
        .data([d])
        .enter()
        .append("foreignObject")
        .attr("x", rectBCR.left - svgBCR.left)
        .attr("y", rectBCR.top - svgBCR.top + 5)
        .attr("width", rectBCR.width)
        .attr("height", rectBCR.height)
        .append("xhtml:p")
        .attr("id", consts.activeEditId)
        .attr("contentEditable", "true")
        .text(d.name)
        .on("mousedown", function(d) {
          d3.event.stopPropagation();
        })
        .on("keydown", function(d) {
          d3.event.stopPropagation();
          if (d3.event.keyCode == consts.ENTER_KEY && !d3.event.shiftKey) {
            this.blur();
          }
        })
        .on("blur", function(d) {
          d.name = this.textContent;
          thisGraph.insertNameToRect(gRect, d);
          d3.select(this.parentElement).remove();
        });
  return d3txt;
};

ConceptMap.prototype.rectMouseUp = function(gRect, d) {
  var thisGraph = this,
      state = thisGraph.state,
      consts = thisGraph.consts;
  // reset the states
  state.shiftRectDrag = false;    
  gRect.classed(consts.connectClass, false);
  
  var mouseDownNode = state.mouseDownNode;
  if (!mouseDownNode) return;

  thisGraph.dragLine.classed("hidden", true);

  if (mouseDownNode !== d) {
    // we're in a different node: create new edge for mousedown edge and add to graph
    var newEdge = {id: thisGraph.getNewId(), name: "new edge", source: mouseDownNode.id, target: d.id};
    var filtRes = thisGraph.edges.filter(function(d) {
      if (d.source === newEdge.target && d.target === newEdge.source) {
        // thisGraph.edges.splice(thisGraph.edges.indexOf(d), 1);
        d.source = newEdge.source;
        d.target = newEdge.target;
        return true;
      }
      return d.source === newEdge.source && d.target === newEdge.target;
    });
    if (!filtRes.length) {
      thisGraph.edges.push(newEdge);
    }
    thisGraph.updateGraph();
  }
  else {
    // we're in the same node
    if (state.justDragged) {
      // dragged, not clicked
      state.justDragged = false;
    }
    else {
      // clicked, not dragged
      if (d3.event.shiftKey) {
        // shift-clicked node: edit text content
        var d3txt = thisGraph.changeTextOfNode(gRect, d);
        var txtNode = d3txt.node();
        thisGraph.selectElementText(txtNode);
        txtNode.focus();
      }
      else {
        if (state.selectedEdge) {
          thisGraph.removeSelectFromEdge();
        }
        var prevNode = state.selectedNode;            
        
        if (!prevNode || prevNode !== d) {
          thisGraph.replaceSelectNode(gRect, d);
        }
        else {
          thisGraph.removeSelectFromNode();
        }
      }
    }
  }
  state.mouseDownNode = null;
  return;
};

ConceptMap.prototype.pathMouseDown = function(gPath, d) {
  var thisGraph = this,
      state = thisGraph.state;
  d3.event.stopPropagation();
  state.mouseDownEdge = d;
  
  if (state.selectedNode) {
    thisGraph.removeSelectFromNode();
  }
  
  /*
  var prevEdge = state.selectedEdge;  
  if (!prevEdge || prevEdge !== d) {
    thisGraph.replaceSelectEdge(gPath, d);
  }
  else {
    thisGraph.removeSelectFromEdge();
  }
  */
};

// place editable text on path in place of svg text
ConceptMap.prototype.changeTextOfPath = function(gPath, d) {
  var thisGraph= this,
      consts = thisGraph.consts,
      htmlEl = gPath.node();
  gPath.selectAll("text").remove();
  var pathBCR = htmlEl.getBoundingClientRect();
  var svgBCR = thisGraph.svg.node().getBoundingClientRect();
  
  // replace with editable content text
  var d3txt = thisGraph.svg.selectAll("foreignObject")
        .data([d])
        .enter()
        .append("foreignObject")
        .attr("x", pathBCR.left + pathBCR.width/2 - 100/2 - svgBCR.left)
        .attr("y", pathBCR.top  + pathBCR.height/2 - 50/2 - svgBCR.top + 5)
        .attr("width", "100")
        .attr("height", "50")
        .append("xhtml:p")
        .attr("id", consts.activeEditId)
        .attr("contentEditable", "true")
        .text(d.name)
        .on("mousedown", function(d) {
          d3.event.stopPropagation();
        })
        .on("keydown", function(d) {
          d3.event.stopPropagation();
          if (d3.event.keyCode == consts.ENTER_KEY && !d3.event.shiftKey) {
            this.blur();
          }
        })
        .on("blur", function(d) {
          d.name = this.textContent;
          thisGraph.insertNameToPath(gPath, d);
          d3.select(this.parentElement).remove();
        });
  return d3txt;
};

ConceptMap.prototype.pathMouseUp = function(gPath, d) {
  var thisGraph = this,
      state = thisGraph.state,
      consts = thisGraph.consts;
  
  var mouseDownEdge = state.mouseDownEdge;
  if (!mouseDownEdge) return;

  // clicked, not dragged
  if (d3.event.shiftKey) {
    // shift-clicked node: edit text content
    var d3txt = thisGraph.changeTextOfPath(gPath, d);
    var txtNode = d3txt.node();
    thisGraph.selectElementText(txtNode);
    txtNode.focus();
  }
  else {
    if (state.selectedNode) {
      thisGraph.removeSelectFromNode();
    }
    var prevEdge = state.selectedEdge;            
    
    if (!prevEdge || prevEdge !== d) {
      thisGraph.replaceSelectEdge(gPath, d);
    }
    else {
      thisGraph.removeSelectFromEdge();
    }
  }

  state.mouseDownEdge = null;
  return;
};

// mousedown on main svg
ConceptMap.prototype.svgMouseDown = function() {
  this.state.graphMouseDown = true;
};

// mouseup on main svg
ConceptMap.prototype.svgMouseUp = function() {
  var thisGraph = this,
      state = thisGraph.state;
  if (state.justScaleTransGraph) {
    // dragged not clicked
    state.justScaleTransGraph = false;
  }
  else if (state.graphMouseDown && d3.event.shiftKey) {
    // clicked not dragged from svg
    var xycoords = d3.mouse(thisGraph.svgG.node()),
        d = {id: thisGraph.getNewId(), name: "new concept", x: xycoords[0], y: xycoords[1]};
    thisGraph.nodes.push(d);
    thisGraph.updateGraph();
    // make name of text immediently editable
    var d3txt = thisGraph.changeTextOfNode(thisGraph.rects.filter(function(dval) {
      return dval.id === d.id;
    }), d),
        txtNode = d3txt.node();
    thisGraph.selectElementText(txtNode);
    txtNode.focus();
  }
  else if (state.shiftRectDrag) {
    // dragged from node
    state.shiftRectDrag = false;
    thisGraph.dragLine.classed("hidden", true);
  }
  state.graphMouseDown = false;
};

// keydown on main svg
ConceptMap.prototype.svgKeyDown = function() {
  var thisGraph = this,
      state = thisGraph.state,
      consts = thisGraph.consts;
  // make sure repeated key presses don't register for each keydown
  if(state.lastKeyDown !== -1) return;

  state.lastKeyDown = d3.event.keyCode;
  var selectedNode = state.selectedNode,
      selectedEdge = state.selectedEdge;

  switch(d3.event.keyCode) {
  case consts.BACKSPACE_KEY:
  case consts.DELETE_KEY:
    d3.event.preventDefault();
    if (selectedNode) {
      thisGraph.nodes.splice(thisGraph.nodes.indexOf(selectedNode), 1);
      thisGraph.spliceEdgesForNode(selectedNode);
      state.selectedNode = null;
      thisGraph.updateGraph();
    } else if (selectedEdge) {
      thisGraph.edges.splice(thisGraph.edges.indexOf(selectedEdge), 1);
      state.selectedEdge = null;
      thisGraph.updateGraph();
    }
    break;
  }
};

ConceptMap.prototype.svgKeyUp = function() {
  this.state.lastKeyDown = -1;
};

// call to propagate changes to graph
ConceptMap.prototype.updateGraph = function() {
  var thisGraph = this,
      consts = thisGraph.consts,
      state = thisGraph.state;
  
  thisGraph.paths = thisGraph.paths.data(thisGraph.edges, function(d) {
    return d.id;
  });
  var paths = thisGraph.paths;
  
  // update existing paths
  paths.classed(consts.selectedClass, function(d) {
    return d === state.selectedEdge;
  });
  paths.select("path").style("marker-end", "url(#end-arrow)")
    //.attr("d", function(d) {
    //  return "M" + d.source.x + "," + d.source.y + "L" + d.target.x + "," + d.target.y;
    //});
    .attr("d", function(d) {
      var source = thisGraph.nodes.filter(function(node) {
          if (node.id === d.source) {
              return node;
          }
      });
      var target = thisGraph.nodes.filter(function(node) {
          if (node.id === d.target) {
              return node;
          }
      });
      return "M" + source[0].x + "," + source[0].y + "L" + target[0].x + "," + target[0].y;
    });
  paths.select("text")
    //.attr("x", function(d) {
    //  return String((d.source.x + d.target.x)/2);
    //})
    .attr("x", function(d) {
      var n = thisGraph.nodes.filter(function(node) {
          if (node.id === d.source || node.id === d.target) {
              return node;
          }
      });
      return String((n[0].x + n[1].x)/2);
    })
    //.attr("y", function(d) {
    //  return String((d.source.y + d.target.y)/2);
    //})
    .attr("y", function(d) {
      var n = thisGraph.nodes.filter(function(node) {
          if (node.id === d.source || node.id === d.target) {
              return node;
          }
      });
      return String((n[0].y + n[1].y)/2);
    })
    .select("tspan").text(function(d) {
      return d.name;
    });

  // add new paths
  var newPaths = paths.enter().append("g");
  newPaths.on("mousedown", function(d) {
      thisGraph.pathMouseDown.call(thisGraph, d3.select(this), d);
      }
    )
    .on("mouseup", function(d) {
      thisGraph.pathMouseUp.call(thisGraph, d3.select(this), d);
    });
  
  newPaths.append("path")
    .style("marker-end","url(#end-arrow)")
    .classed("link", true)
    //.attr("d", function(d) {
    //  return "M" + d.source.x + "," + d.source.y + "L" + d.target.x + "," + d.target.y;
    //});
    .attr("d", function(d) {
      var source = thisGraph.nodes.filter(function(node) {
          if (node.id === d.source) {
              return node;
          }
      });
      var target = thisGraph.nodes.filter(function(node) {
          if (node.id === d.target) {
              return node;
          }
      });
      return "M" + source[0].x + "," + source[0].y + "L" + target[0].x + "," + target[0].y;
    });

  newPaths.each(function(d) {
      thisGraph.insertNameToPath(d3.select(this), d);
  });
  
  // remove old links
  paths.exit().remove();
  
  // update existing nodes
  thisGraph.rects = thisGraph.rects.data(thisGraph.nodes, function(d) {
    return d.id;
  });
  var rects = thisGraph.rects;
  rects.attr("transform", function(d) {return "translate(" + d.x + "," + d.y + ")";});
  
  // add new nodes
  var newRects= rects.enter().append("g");

  newRects.classed(consts.rectGClass, true)
    .attr("transform", function(d) {return "translate(" + d.x + "," + d.y + ")";})
    .on("mouseover", function(d) {        
      if (state.shiftRectDrag) {
        d3.select(this).classed(consts.connectClass, true);
      }
    })
    .on("mouseout", function(d) {
      d3.select(this).classed(consts.connectClass, false);
    })
    .on("mousedown", function(d) {
      thisGraph.rectMouseDown.call(thisGraph, d3.select(this), d);
    })
    .on("mouseup", function(d) {
      thisGraph.rectMouseUp.call(thisGraph, d3.select(this), d);
    })
    .call(thisGraph.drag);

  newRects.append("rect")
    .attr("width", String(consts.rectWidth))
    .attr("height", String(consts.rectHeight))
    .attr("x", String(-consts.rectWidth/2))
    .attr("y", String(-consts.rectHeight/2));

  newRects.each(function(d) {
    thisGraph.insertNameToRect(d3.select(this), d);
  });

  // remove old nodes
  thisGraph.rects.exit().remove();
};

ConceptMap.prototype.zoomed = function() {
  this.state.justScaleTransGraph = true;
  d3.select("." + this.consts.graphClass)
    .attr("transform", "translate(" + d3.event.translate + ") scale(" + d3.event.scale + ")"); 
};
