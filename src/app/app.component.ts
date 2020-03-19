import { Component, Inject } from '@angular/core';
import {Chart} from 'chart.js';
import 'chartjs-plugin-zoom'
import 'chartjs-plugin-streaming'
import { SessionDataService } from './session-data.service';
import { WebsocketService } from './websocket.service';
import {SESSION_STORAGE, WebStorageService} from 'angular-webstorage-service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})



export class AppComponent {
  
constructor( @Inject(SESSION_STORAGE)
  private service:SessionDataService, 
  private dataStream : WebsocketService,
  private browserStorage : WebStorageService ){}

  title = 'gui';
  linechart1: any;
  linechart : any;
  data =[];
  public sessionId:any=[]

  // dates = ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'];
  // readings = [12, 19, 3, 5, 2, 3];
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
        // type: 'realtime',
        // realtime: {         // per-axis options
        //   duration: 20000,    // data in the past 20000 ms will be displayed
        //   // refresh: 1000,      // onRefresh callback will be called every 1000 ms
        //   delay: 1,        // delay of 1000 ms, so upcoming values are known before plotting a line
        //   pause: false,       // chart is not paused
        //   ttl: undefined,     // data will be automatically deleted as it disappears off the chart
        // },
        ticks:{display:false}
      }]
    },
    plugins: {
      streaming: {            // per-chart option
        frameRate: 30       // chart is drawn 30 times every second
      },
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
    if (this.browserStorage.get('id')){
      this.getData(this.browserStorage.get('id'));
    }
    // this.getData("5e731175f8d4ba4ad4fa71dd");
    this.dataStream.getMessages().subscribe(
      (message: any[]) => {
        // console.log(message)
        if(message['updatedFields']){
          for (var key in message['updatedFields']) {
            // console.log("Key: " + key);
            this.addData(this.linechart1,message['updatedFields'][key]['time_stamp'], message['updatedFields'][key]['value'])
            // console.log("Value: " + message['updatedFields'][key]['value']);
        }
        }
        console.log(this.data,this.linechart1.data.datasets[0].data)
    });
    // // create the charts
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
          labels: [],
          datasets: [{
              // label: '# of Votes',
              data: [],
              // backgroundColor:'rgba(255, 99, 132, 0.0)',
              // borderColor: 'rgba(255, 159, 64, 1)',
              // borderWidth: 1
          }]
      },
      options: this.options
    });
  }


randomData(){
  var i;
  for (i= 0; i<10; i++){
    let time = 10*(Math.random());
    let data = 15*(Math.random());
    // this.addData(this.linechart1,time,data);
    this.service.addReading(time.toString(),data);
  }
}

getData(id){
  this.service.getReadings(id);
  this.service.readingSubject.subscribe(data =>{
    if (data !== undefined){
      for (var p in data['Pressure']){
        this.addData(this.linechart1,data['Pressure'][p]['time_stamp'], data['Pressure'][p]['value']);
      }
      for (var p in data['Temperature']){
        this.addData(this.linechart1,data['Temperature'][p]['time_stamp'], data['Temperature'][p]['value']);
      }
    }
  }

  );


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
ngOnDestroy() {
  this.dataStream.getMessages().unsubscribe();
}

}
