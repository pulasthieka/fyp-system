import { Injectable, Inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import {SESSION_STORAGE, WebStorageService} from 'angular-webstorage-service';
import { Observable, BehaviorSubject, Subject } from 'rxjs';


@Injectable({
  providedIn: 'root'
})
export class SessionDataService {

  uri = 'http://localhost:9000/session';
  id = "5e731175f8d4ba4ad4fa71dd";
  
  readingSubject = new Subject();
  readings = this.readingSubject.subscribe();

  constructor(@Inject(SESSION_STORAGE) private http : HttpClient, 
  private browserStorage : WebStorageService ) { }

  newRecording(){
    this.http.post(`${this.uri}/new`, {})
    .subscribe(res => {
        this.browserStorage.set('sessionId', res['id']);
        this.id = res['id'];
        console.log('New Record Created',res['status'], this.id)});
    
  }
  addReading(time, reading) {
    const session =  {Pressure:{
      time_stamp: time,
      value: reading
    },
    Temperature :{
      time_stamp:time,
      value:(reading-10)
    }
  };
    // console.log(session);
    this.http.post(`${this.uri}/add/${this.id}`, session)
    .subscribe(res => {
          this.id = res['id'];
          console.log('Done',res['status'], this.id)});
  }

  getReadings(id){
    this.http.get(`${this.uri}/get/${id}`).subscribe(
      res =>{
        this.readingSubject.next(res);
        this.id = id;
      }
    )
  }

}
