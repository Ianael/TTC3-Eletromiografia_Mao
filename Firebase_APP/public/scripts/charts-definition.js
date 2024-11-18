// Gera gráficos quando a página carrega
window.addEventListener('load', onload);

function onload(event) {
    chartF = createFlexChart();
    chartE = createExtChart();
    chartP = createPolChart();
}

// Cria gráfico do FLEXOR
function createFlexChart() {
    var chart = new Highcharts.Chart({
        chart: {
            renderTo: 'chart-flex',
            type: 'spline'
        },
        series: [
            {
                name: 'MyoWare Flexor'
            }
        ],
        title: {
            text: undefined
        },
        plotOptions: {
            line: {
                animation: false,
                dataLabels: {
                    enabled: true
                }
            },
            series: {
                color: '#059E8A'
            }
        },
        xAxis: {
            type: 'datetime',
            dateTimeLabelFormats: { second: '%H:%M:%S' }
        },
        yAxis: {
            title: {
                text: 'Sinal EMG'
            }
        },
        credits: {
            enabled: false
        }
    });

    return chart;
}

// Cria gráfico do EXTENSOR
function createExtChart() {
    var chart = new Highcharts.Chart({
        chart: {
            renderTo: 'chart-ext',
            type: 'spline'
        },
        series: [{
            name: 'MyoWare Extensor'
        }],
        title: {
            text: undefined
        },
        plotOptions: {
            line: {
                animation: false,
                dataLabels: {
                    enabled: true
                }
            },
            series: {
                color: '#8A2F3C'
            }
        },
        xAxis: {
            type: 'datetime',
            dateTimeLabelFormats: { second: '%H:%M:%S' }
        },
        yAxis: {
            title: {
                text: 'Sinal EMG'
            }
        },
        credits: {
            enabled: false
        }
    });

    return chart;
}

// Cria gráfico do POLEGAR
function createPolChart() {
    var chart = new Highcharts.Chart({
        chart: {
            renderTo: 'chart-pol',
            type: 'spline'
        },
        series: [{
            name: 'MyoWare Polegar'
        }],
        title: {
            text: undefined
        },
        plotOptions: {
            line: {
                animation: false,
                dataLabels: {
                    enabled: true
                }
            },
            series: {
                color: '#E1E437'
            }
        },
        xAxis: {
            type: 'datetime',
            dateTimeLabelFormats: { second: '%H:%M:%S' }
        },
        yAxis: {
            title: {
                text: 'Sinal EMG'
            }
        },
        credits: {
            enabled: false
        }
    });

    return chart;
}