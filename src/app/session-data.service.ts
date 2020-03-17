import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import Reading from './reading'

@Injectable({
  providedIn: 'root'
})
export class SessionDataService {

  uri = 'http://localhost:500/session';

  constructor(private http : HttpClient) { }

  addReading(time, reading) {
    const session ={Pressure:{
      time_stamp: time,
      value: reading
    },
    Temperature :{
      time_stamp:'0',
      value:0
    }
  };
    console.log(session);
    this.http.post(`${this.uri}/add`, session)
        .subscribe(res => console.log('Done'));
  }

}
