const DEBUG = true;

var globalMCharts = new Array();

Highcharts.setOptions({
  global: {
    useUTC: false
  }
});

function series() {
    let names = ['AF3', 'F7', 'F3', 'FC5', 'T7', 'P7', 'O1', 'O2', 'P8', 'T8', 'FC6', 'F4', 'F8', 'AF4'];
    let array = [];
    for (let name of names) {
        array.push({name: name,
                    data: [],
                    visible: true,
                    //color: '#4572A7',
                    tooltip: {
                        valueDecimals: 2
                    }
                   });
    }

    for (let e of array) {
        let disable = (e.name == 'FC5') ||
                      (e.name == 'T7' ) ||
                      (e.name == 'P7' ) ||
                      (e.name == 'O1' ) ||
                      (e.name == 'O2' ) ||
                      (e.name == 'P8' ) ||
                      (e.name == 'T8' );
        if (disable) e.visible = false;
    }

    return array;
}


function log(s) {
    if (DEBUG) console.log(s);
}

function debug(f) {
    if (DEBUG) f();
}


function getCheckboxId(id) {
    return id + "-checkbox";
}


function getButtonId(id) {
    return id + "-button";
}


function getStateId(id) {
    return id + "-state";
}


function getChartId(id) {
    return id + "-chart";
}


function ManagedChart(id, data) {
    let html =
    "<div class='panel panel-info' id='{{id}}'>" +
        "<div class='panel-heading'>" +
            "<div class='row'>" +
                "<div class='col-md-4'>" +
                    "<div class='input-group'>" +
                        "<span class='input-group-addon'>" +
                            "<input type='checkbox' id='{{checkbox-id}}' data-group='{{id}}' onclick='GUI_showHideChart(this)' checked>" +
                        "</span>" +
                        "<span class='input-group-addon'>{{id}}</span>" +
                        "<label class='form-control' id='{{state-id}}' data-group='{{id}}'>unknown</label>" +
                    "</div>" +
                "</div>" +
                "<div class='col-md-8'>" +
                    "<button class='btn btn-danger pull-right' id='{{button-id}}' data-group='{{id}}' onclick='GUI_removeChart(this)'>" +
                        "<span class='glyphicon glyphicon glyphicon-remove'></span>" +
                    "</button>" +
                "</div>" +
            "</div>" +
        "</div>" +
        "<div class='panel-body chart' id='{{chart-id}}' data-group='{{id}}'></div>" +
    "</div>";

    let output = html;
    output = output.replace(new RegExp("{{id}}", "g"), id);
    output = output.replace(new RegExp("{{checkbox-id}}", "g"), getCheckboxId(id));
    output = output.replace(new RegExp("{{state-id}}", "g"), getStateId(id));
    output = output.replace(new RegExp("{{chart-id}}", "g"), getChartId(id));
    output = output.replace(new RegExp("{{button-id}}", "g"), getButtonId(id));

    let top = document.getElementById("top");
    top.insertAdjacentHTML("beforeend", output);

    let chart = new Highcharts.chart(getChartId(id), {
        chart: {
            type: 'spline',
            animation: false,
        },
        title: {
            text: 'Signals'
        },
        xAxis: {
            type: 'datetime',
            ordinal: false,
            //tickInterval: 1000
        },
        yAxis: {
            min: 0,
            title: {
                text: 'Signal'
            }
        },
        legend: {
            enabled: true
        },
        // credits: {
        //     enabled: true,
        //     text: 'Credit'
        // },
        plotOptions: {
            spline: {
                marker: {
                    enabled: false
                }
            }
        },
        tooltip: {
            shared: true
        },
        series: series()
    });

    this.id = id;
    this.chart = chart;
    this.cache = new Array();

    // debug(() => {
    //     setInterval(() => {
    //         chart.series[0].addPoint(Math.random() * 100, true, false);
    //     }, 1000);
    // });
}


function findChart(charts, id) {
    for (let ch of charts) {
        if (ch.id == id) return ch;
    }
    return null;
}


function removeChart(charts, id) {
    for (let index in charts) {
        if (charts[index].id == id) {
            charts.splice(index, 1);
            return;
        }
    }
}


function addData(mchart, data) {
    const MAXPOINTS = 20;

    let time = data.time;
    let ss = series();
    for (let i in ss) {
        if (mchart.cache[i] === undefined) {
            mchart.cache[i] = new Array();
        }

        let point = [time, data[ss[i].name]];
        mchart.cache[i].push(point);
        if (mchart.cache[i].length > MAXPOINTS) {
            mchart.cache[i].shift();
        }

        let serie = mchart.chart.series[i];
        if (serie.visible) {
            let shift = serie.data.length > MAXPOINTS;
            serie.addPoint(point, false, shift);
        } else {
            serie.setData(mchart.cache[i], false);
        }
    }

    document.getElementById(getStateId(mchart.id)).innerHTML = data.state;
}


function GUI_showHideChart(e) {
    let chart = document.getElementById(getChartId(e.getAttribute("data-group")));
    if (e.checked) {
        chart.style.display = "block";
    } else {
        chart.style.display = "none";
    }
}


function GUI_removeChart(e) {
    let id = e.getAttribute("data-group");
    let chart = document.getElementById(id);
    chart.parentNode.removeChild(chart);

    removeChart(globalMCharts, id);
}


window.onload = function() {
    //new ManagedChart("x", null);
    setInterval(() => {
        if (globalMCharts.length > 10) return;

        $.getJSON("data.json")
            .done((json) => {
                //log(json);
                for (let e of json) {
                    let mchart = findChart(globalMCharts, e.id);
                    if (mchart == null) {
                        globalMCharts.push(new ManagedChart(e.id, e.data));
                    } else {
                        addData(mchart, e.data);
                    }
                }

                for (let mchart of globalMCharts) {
                    mchart.chart.redraw();
                }
            })
            .fail((jqxhr, textStatus, error) => {
                log(jqxhr);
            });
    }, 1000/2);
};