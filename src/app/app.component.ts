import { Component } from '@angular/core';
import {Chart} from 'chart.js';
import 'chartjs-plugin-zoom'
import { SessionDataService } from './session-data.service';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})



export class AppComponent {
  
constructor(private service:SessionDataService){}

  title = 'gui';
  linechart1: any;
  linechart : any;
  dates = ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'];
  readings = [12, 19, 3, 5, 2, 3];
  options = {
    elements: {
      line: { tension: 0 } // for drawing straight lines
    },
    maintainAspectRatio: false,
    scales: {
      yAxes: [{
          ticks: { beginAtZero: true} // y axis begins from 0 otherwise from the lowest data value
      }],
      xAxes:[{
        ticks:{visible:false}
      }]
    },
    plugins: {
      zoom: {// Container for pan options
        pan: {
          enabled: true,
          mode: 'xy',
          rangeMin: {
            x: null,
            y: null
          },
          rangeMax: {
            x: null,
            y: null
          },
          speed: 10,
          threshold: 5,
        },

        // Container for zoom options
        zoom: {
          enabled: true,
          drag: false,  
          mode: 'x',
          rangeMin: {
            // Format of min zoom range depends on scale type
            x: null,
            y: null
          },
          rangeMax: {
            // Format of max zoom range depends on scale type
            x: null,
            y: null
          },
          speed: 0.1,
          sensitivity: 3,
        }
      }
    }
  }

  ngOnInit() {
    // create the charts
    // this.linechart = new Chart('myChart', {
    //   // The type of chart we want to create
    //   type: 'line',
  
    //   // The data for our dataset
    //   data: {
    //       labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July'],
    //       datasets: [{
    //           label: 'My First dataset',
    //           backgroundColor: 'rgb(255, 99, 132)',
    //           borderColor: 'rgb(255, 99, 132)',
    //           data: [0, 10, 5, 50, 20, 30, 45]
    //       }]
    //   },
  
    //   // Configuration options go here
    //   options: {
    //     maintainAspectRatio:false,
    //   }
    // });

    this.linechart1 = new Chart('myChart1',{
      type: 'line',
      data: {
          labels: this.dates,
          datasets: [{
              label: '# of Votes',
              data: this.readings,
              backgroundColor:'rgba(255, 99, 132, 0.0)',
              borderColor: 'rgba(255, 159, 64, 1)',
              borderWidth: 1
          }]
      },
      options: this.options
    });
  }


randomData(){
  let time = 10*(Math.random());
  let data = 15*(Math.random());
  this.addData(this.linechart1,time,data);
  this.service.addReading(time.toString(),data);
}
  // function to add data to graph
addData(chart, label, data) {
    chart.data.labels.push(label);
    chart.data.datasets.forEach((dataset) => {
        dataset.data.push(data);
    });
    chart.update();
}

// function to remove
removeData(chart) {
    chart.data.labels.pop();
    chart.data.datasets.forEach((dataset) => {
        dataset.data.pop();
    });
    chart.update();
}

// example code on push data
// onReceive(event) {
//   // append the new data to the existing chart data
//   myChart.data.datasets[0].data.push({
//       x: event.timestamp,
//       y: event.value
//   });
//   // update chart datasets keeping the current animation
//   myChart.update({
//       preservation: true
//   });
// }

}
