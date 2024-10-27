// Create the charts when the web page loads
window.addEventListener('load', onload);

function onload(event){
  chartF = createFirstChart();
  chartS = createSecondChart();
  chartT = createThumbChart();
}

// Create First Chart
function createFirstChart() {
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-first',
      type: 'spline' 
    },
    series: [
      {
        name: 'Myoware Flexor'
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

// Create Second Chart
function createSecondChart(){
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-second',
      type: 'spline'  
    },
    series: [{
      name: 'Myoware Extensor'
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
        color: '#50b8b4' 
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

// Create Thumb Chart
function createThumbChart() {
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-thumb',
      type: 'spline'  
    },
    series: [{
      name: 'Myoware Polegar'
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
        color: '#A62639' 
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